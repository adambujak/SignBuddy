import sign_buddy_pb2 as sbp

sample = sbp.SBPSample()

sample.touch_data.touch_1 = 1
sample.touch_data.touch_2 = 1
sample.touch_data.touch_3 = 1
sample.touch_data.touch_4 = 1
sample.touch_data.touch_5 = 1
sample.touch_data.touch_6 = 1
sample.touch_data.touch_7 = 1
sample.touch_data.touch_8 = 1
sample.touch_data.touch_9 = 1
sample.touch_data.touch_10 = 1
sample.touch_data.touch_11 = 1
sample.touch_data.touch_12 = 1

sample.flex_data.flex_thumb = 3
sample.flex_data.flex_index = 4
sample.flex_data.flex_middle = 5
sample.flex_data.flex_ring = 6
sample.flex_data.flex_little = 89

sample.imu_data.eul_h = 3
sample.imu_data.eul_r = 3
sample.imu_data.eul_p = 3
sample.imu_data.lin_acc_x = 4
sample.imu_data.lin_acc_y = 4
sample.imu_data.lin_acc_z = 4

sample.sample_id = 3

output = sample.SerializeToString()
print(output)
newSample = sbp.SBPSample()
newSample.ParseFromString(output)
print(newSample)

