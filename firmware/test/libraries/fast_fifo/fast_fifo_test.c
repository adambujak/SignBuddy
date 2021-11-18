#include "fifo.h"
#include "unity.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG
#define LOG_DEBUG(fmt, ...)        \
  do {                             \
    printf((fmt), ## __VA_ARGS__); \
  } while (0)
#else
#define LOG_DEBUG(...)    do {} while (0)
#endif

static void assert_buffer_is_val(uint8_t *buffer, uint32_t size, uint8_t val)
{
  for (uint32_t i = 0; i < size; i++) {
    TEST_ASSERT_EQUAL(buffer[i], val);
  }
}

static void fill_buffer_random(uint8_t *buffer, uint32_t size)
{
  for (uint32_t i = 0; i < size; i++) {
    buffer[i] = rand() & 0xFF;
  }
}

static void print_buffer(uint8_t *buffer, uint32_t size)
{
  (void)buffer;
  LOG_DEBUG("\n{\n");
  for (uint32_t i = 0; i < size; i++) {
    LOG_DEBUG("%x, ", buffer[i]);
  }
  LOG_DEBUG("\n}\n\n");
}

static void print_buffer_detailed(uint8_t *buffer, uint32_t size)
{
  (void)buffer;
  LOG_DEBUG("\n{\n");
  for (uint32_t i = 0; i < size; i++) {
    LOG_DEBUG("[%u]: %x, \n", i, buffer[i]);
  }
  LOG_DEBUG("\n}\n\n");
}

void setUp(void)
{}

void tearDown(void)
{}

void test_read(void)
{
  #define SIZE    1024
  uint8_t buffer[SIZE] = { 0 };
  fifo_t fifo;
  TEST_ASSERT_EQUAL(0, fifo_init(&fifo, buffer, SIZE));
  #undef SIZE

  #define READ_SIZE    128

  uint8_t write_data[READ_SIZE];
  fill_buffer_random(write_data, READ_SIZE);

  TEST_ASSERT_EQUAL(READ_SIZE, fifo_push(&fifo, write_data, READ_SIZE));

  uint8_t read_data[READ_SIZE];
  TEST_ASSERT_EQUAL(READ_SIZE, fifo_pop(&fifo, read_data, READ_SIZE));

  TEST_ASSERT_EQUAL(0, memcmp(write_data, read_data, READ_SIZE));
  TEST_ASSERT_EQUAL(0, fifo_pop(&fifo, read_data, 1));
  TEST_ASSERT_EQUAL(0, fifo_bytes_used_cnt_get(&fifo));
  #undef READ_SIZE
}

void test_write(void)
{
  #define SIZE    1024
  uint8_t buffer[SIZE] = { 0 };
  fifo_t fifo;
  TEST_ASSERT_EQUAL(0, fifo_init(&fifo, buffer, SIZE));

  #define WRITE_SIZE    128
  uint8_t write_data[WRITE_SIZE];

  for (uint32_t i = 0; i < (SIZE / WRITE_SIZE); i++) {
    fill_buffer_random(write_data, WRITE_SIZE);

    LOG_DEBUG("fifo bytes used: %d\r\n", fifo.bytes_used);
    LOG_DEBUG("fifo size: %d\r\n", fifo.size);
    uint32_t val = fifo_push(&fifo, write_data, WRITE_SIZE);
    LOG_DEBUG("fifo bytes used: %d\r\n", fifo.bytes_used);
    LOG_DEBUG("fifo size: %d\r\n", fifo.size);
    TEST_ASSERT_EQUAL(WRITE_SIZE, val);

    TEST_ASSERT_EQUAL(0, memcmp(&buffer[WRITE_SIZE * i], write_data, WRITE_SIZE));
    TEST_ASSERT_EQUAL(WRITE_SIZE * (i + 1), fifo_bytes_used_cnt_get(&fifo));
  }

  #undef READ_SIZE
  #undef SIZE
}

void test_read_empty_buffer(void)
{
  #define SIZE    256
  uint8_t data[SIZE] = { 0 };
  fifo_t fifo;
  TEST_ASSERT_EQUAL(0, fifo_init(&fifo, data, SIZE));
  #undef SIZE
  uint8_t read_data[3] = { 0 };

  TEST_ASSERT_EQUAL(0, fifo_bytes_used_cnt_get(&fifo));
  TEST_ASSERT_EQUAL(0, fifo_pop(&fifo, read_data, 3));
}

void test_write_more_than_size_of_buffer(void)
{
  #define SIZE    128
  uint8_t buffer[SIZE];
  fifo_t fifo;
  TEST_ASSERT_EQUAL(0, fifo_init(&fifo, buffer, SIZE));

  memset(buffer, 0, SIZE);
  assert_buffer_is_val(buffer, SIZE, 0);

  uint8_t write_data[SIZE * 2];
  memset(write_data, 1, SIZE * 2);
  assert_buffer_is_val(write_data, SIZE * 2, 1);

  TEST_ASSERT_EQUAL(0, fifo_push(&fifo, write_data, SIZE * 2));
  TEST_ASSERT_EQUAL(0, fifo_bytes_used_cnt_get(&fifo));

  assert_buffer_is_val(buffer, SIZE, 0);
  #undef SIZE
}

void test_write_wraparound(void)
{
  #define SIZE    128
  uint8_t buffer[SIZE] = { 0 };
  memset(buffer, 0, SIZE);
  fifo_t fifo;
  TEST_ASSERT_EQUAL(0, fifo_init(&fifo, buffer, SIZE));

  #define WRITE_SIZE_0    50
  #define WRITE_SIZE_1    82
  #define WRITE_SIZE_2    12

  uint8_t write_data0[WRITE_SIZE_0];
  uint8_t write_data1[WRITE_SIZE_1];
  uint8_t write_data2[WRITE_SIZE_2];

  uint8_t read_data0[WRITE_SIZE_0];
  uint8_t read_data1[WRITE_SIZE_1];
  uint8_t read_data2[WRITE_SIZE_2];

  fill_buffer_random(write_data0, WRITE_SIZE_0);
  fill_buffer_random(write_data1, WRITE_SIZE_1);
  fill_buffer_random(write_data2, WRITE_SIZE_2);

  TEST_ASSERT_EQUAL(WRITE_SIZE_0, fifo_push(&fifo, write_data0, WRITE_SIZE_0));
  TEST_ASSERT_EQUAL(WRITE_SIZE_0, fifo_bytes_used_cnt_get(&fifo));
  TEST_ASSERT_EQUAL(WRITE_SIZE_0, fifo_pop(&fifo, read_data0, WRITE_SIZE_0));
  TEST_ASSERT_EQUAL(0, fifo_bytes_used_cnt_get(&fifo));
  TEST_ASSERT_EQUAL(0, memcmp(write_data0, read_data0, WRITE_SIZE_0));

  TEST_ASSERT_EQUAL(WRITE_SIZE_1, fifo_push(&fifo, write_data1, WRITE_SIZE_1));
  TEST_ASSERT_EQUAL(WRITE_SIZE_1, fifo_bytes_used_cnt_get(&fifo));

  TEST_ASSERT_EQUAL(WRITE_SIZE_2, fifo_push(&fifo, write_data2, WRITE_SIZE_2));
  TEST_ASSERT_EQUAL(WRITE_SIZE_1 + WRITE_SIZE_2, fifo_bytes_used_cnt_get(&fifo));

  TEST_ASSERT_EQUAL(WRITE_SIZE_1, fifo_pop(&fifo, read_data1, WRITE_SIZE_1));
  TEST_ASSERT_EQUAL(WRITE_SIZE_2, fifo_bytes_used_cnt_get(&fifo));

  TEST_ASSERT_EQUAL(WRITE_SIZE_2, fifo_pop(&fifo, read_data2, WRITE_SIZE_2));
  TEST_ASSERT_EQUAL(0, fifo_bytes_used_cnt_get(&fifo));

  TEST_ASSERT_EQUAL(0, memcmp(write_data1, read_data1, WRITE_SIZE_1));
  TEST_ASSERT_EQUAL(0, memcmp(write_data2, read_data2, WRITE_SIZE_2));

  #undef WRITE_SIZE_1
  #undef WRITE_SIZE_2
  #undef SIZE
}

void test_peek_and_pop_wraparound(void)
{
  #define SIZE    128
  uint8_t buffer[SIZE] = { 0 };
  memset(buffer, 0, SIZE);
  fifo_t fifo;
  TEST_ASSERT_EQUAL(0, fifo_init(&fifo, buffer, SIZE));

  #define WRITE_SIZE_0    50
  #define WRITE_SIZE_1    82
  #define WRITE_SIZE_2    12

  uint8_t write_data0[WRITE_SIZE_0];
  uint8_t write_data1[WRITE_SIZE_1];

  fill_buffer_random(write_data0, WRITE_SIZE_0);
  fill_buffer_random(write_data1, WRITE_SIZE_1);

  // Offset write index to middle of buffer
  TEST_ASSERT_EQUAL(WRITE_SIZE_0, fifo_push(&fifo, write_data0, WRITE_SIZE_0));
  TEST_ASSERT_EQUAL(WRITE_SIZE_0, fifo_bytes_used_cnt_get(&fifo));
  fifo_drop(&fifo, WRITE_SIZE_0);
  TEST_ASSERT_EQUAL(0, fifo_bytes_used_cnt_get(&fifo));

  #define READ_SIZE    WRITE_SIZE_1
  uint8_t read_data[READ_SIZE];

  TEST_ASSERT_EQUAL(WRITE_SIZE_1, fifo_push(&fifo, write_data1, WRITE_SIZE_1));
  TEST_ASSERT_EQUAL(WRITE_SIZE_1, fifo_bytes_used_cnt_get(&fifo));

  TEST_ASSERT_EQUAL(WRITE_SIZE_1, fifo_peek(&fifo, read_data, WRITE_SIZE_1));

  print_buffer_detailed(buffer, SIZE);
  print_buffer(write_data1, WRITE_SIZE_1);

  TEST_ASSERT_EQUAL(WRITE_SIZE_1, fifo_bytes_used_cnt_get(&fifo));
  TEST_ASSERT_EQUAL(0, memcmp(write_data1, read_data, READ_SIZE));


  TEST_ASSERT_EQUAL(READ_SIZE, fifo_peek(&fifo, read_data, READ_SIZE));
  TEST_ASSERT_EQUAL(WRITE_SIZE_1, fifo_bytes_used_cnt_get(&fifo));
  TEST_ASSERT_EQUAL(0, memcmp(write_data1, read_data, READ_SIZE));

  fifo_drop(&fifo, 10);

  TEST_ASSERT_EQUAL(WRITE_SIZE_1 - 10, fifo_bytes_used_cnt_get(&fifo));
  TEST_ASSERT_EQUAL(WRITE_SIZE_1 - 10, fifo_pop(&fifo, read_data, WRITE_SIZE_1 - 10));
  TEST_ASSERT_EQUAL(0, memcmp(write_data1 + 10, read_data, WRITE_SIZE_1 - 10));
  TEST_ASSERT_EQUAL(0, fifo_bytes_used_cnt_get(&fifo));

  #undef WRITE_SIZE_0
  #undef WRITE_SIZE_1
  #undef READ_SIZE
  #undef SIZE
}
