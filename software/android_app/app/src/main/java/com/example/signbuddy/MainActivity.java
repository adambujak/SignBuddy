package com.example.signbuddy;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.Manifest;
import android.annotation.SuppressLint;
import android.app.AlertDialog;
import android.bluetooth.BluetoothGatt;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ProgressBar;
import android.widget.Toast;

import com.clj.fastble.BleManager;
import com.clj.fastble.callback.BleGattCallback;
import com.clj.fastble.callback.BleNotifyCallback;
import com.clj.fastble.callback.BleScanCallback;
import com.clj.fastble.callback.BleWriteCallback;
import com.clj.fastble.data.BleDevice;
import com.clj.fastble.exception.BleException;
import com.clj.fastble.scan.BleScanRuleConfig;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.locks.ReentrantLock;

public class MainActivity extends AppCompatActivity {

    private final int FINE_LOCATION_CODE = 1;
    private final String BLE_NAME = "Sign Buddy BLE";
    private final String UUID_SERVICE = "6E400001-B5A3-F393-E0A9-E50E24DCCA9E";
    private final String UUID_CHARACTERISTIC_WRITE = "6E400002-B5A3-F393-E0A9-E50E24DCCA9E";
    private final String UUID_CHARACTERISTIC_NOTIFY = "6E400003-B5A3-F393-E0A9-E50E24DCCA9E";
    private final byte[] CMD_SAMPLE_ONCE = {0x01};
    private final byte[] CMD_SAMPLE_PERIODIC = {0x02};
    private final byte[] CMD_RESET_IMU = {0x03};
    private final int MAX_SAMPLES = 40;
    private final byte MSG_SYNC = 0x16;
    private final byte MID_SAMPLE = 0x01;

    private Button connectButton;
    private Button resetButton;
    private ProgressBar connectProgress;
    private Button collectButton;
    private BleDevice SignBuddy;
    private ReentrantLock samplesLock = new ReentrantLock();
    private List<SignBuddyProto.SBPSample> samples = new ArrayList<>();
    private ByteArrayOutputStream byteBuffer = new ByteArrayOutputStream();
    private int num_samples;
    private int msg_length;
    private int skip_crc;
    private boolean next_byte_is_length;
    private String gestureLetter = "DEFAULT";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button lessonButton = findViewById(R.id.lessonButton);
        lessonButton.setOnClickListener(this::openLessonActivity);

        Button quizButton = findViewById(R.id.quizButton);
        quizButton.setOnClickListener(this::openQuizActivity);

        connectProgress = findViewById(R.id.connectProgress);
        connectButton = findViewById(R.id.connectButton);
        connectButton.setOnClickListener(this::connectButtonCallback);

        resetButton = findViewById(R.id.resetButton);
        resetButton.setOnClickListener(this::resetButtonCallback);

        collectButton = findViewById(R.id.collectButton);
        collectButton.setOnClickListener(this::collectButtonCallback);

        BleManager.getInstance().init(getApplication());
        BleManager.getInstance()
                .enableLog(false)
                .setReConnectCount(1, 5000)
                .setOperateTimeout(5000);

        if (!BleManager.getInstance().isSupportBle()) {
            connectButton.setEnabled(false);
            connectButton.setText(R.string.BLE_not_supported);
        }

        if (!BleManager.getInstance().isBlueEnable()) {
            connectButton.setText(R.string.enable_bluetooth);
        }

        if (ContextCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION) == PackageManager.PERMISSION_GRANTED) {
            connectButton.setVisibility(View.VISIBLE);
        } else {
            requestFineLocation();
        }

        BleScanRuleConfig scanRuleConfig = new BleScanRuleConfig.Builder()
                .setScanTimeOut(3000)
                .build();

        BleManager.getInstance().initScanRule(scanRuleConfig);
    }

    @Override
    public void onBackPressed() {
        super.onBackPressed();
        BleManager.getInstance().disconnect(SignBuddy);
    }

    private void openLessonActivity(View v) {
        Intent intent = new Intent(this, LessonMenuActivity.class);
        startActivity(intent);
    }

    private void openQuizActivity(View v) {
        Intent intent = new Intent(this, QuizMenuActivity.class);
        startActivity(intent);
    }

    private void collectButtonCallback(View v) {
        EditText collectLetter = findViewById(R.id.collectLetter);
        gestureLetter = collectLetter.getText().toString();
        if (gestureLetter.matches("")) {
            Toast.makeText(this, "No letter specified!", Toast.LENGTH_SHORT).show();
        } else {
            collectButton.setEnabled(false);
            collectButton.setText(R.string.collecting);
            byte[] cmd;
            // If the letter is J or Z then we must do periodic sampling to track movement
            if ("jzJZ".contains(gestureLetter)) {
                num_samples = MAX_SAMPLES;
                cmd = CMD_SAMPLE_PERIODIC;
            } else {
                num_samples = 1;
                cmd = CMD_SAMPLE_ONCE;
            }
            BleManager.getInstance().write(SignBuddy, UUID_SERVICE, UUID_CHARACTERISTIC_WRITE, cmd, new BleWriteCallback() {
                @Override
                public void onWriteSuccess(int current, int total, byte[] justWrite) {
                    Toast.makeText(MainActivity.this, "Starting collection!", Toast.LENGTH_SHORT).show();
                }

                @Override
                public void onWriteFailure(BleException exception) {
                    Toast.makeText(MainActivity.this, "Failed to start collection!", Toast.LENGTH_SHORT).show();
                    collectButton.setEnabled(true);
                    collectButton.setText(R.string.collect);
                }
            });
        }
    }

    private void connectButtonCallback(View v) {
        if (connectButton.getText().equals(getString(R.string.enable_bluetooth))) {
            connectButton.setEnabled(false);
            connectButton.setText(R.string.enabling_bluetooth);
            BleManager.getInstance().enableBluetooth();
            connectButton.setEnabled(true);
            connectButton.setText(R.string.connect_to_device);
        } else {
            connectButton.setEnabled(false);
            connectButton.setText(R.string.scanning);
            connectProgress.setVisibility(View.VISIBLE);
            BleManager.getInstance().scan(new BleScanCallback() {
                @Override
                public void onScanFinished(List<BleDevice> scanResultList) {
                    if (SignBuddy == null) {
                        Toast.makeText(MainActivity.this, "Could not find device", Toast.LENGTH_SHORT).show();
                        connectButton.setEnabled(true);
                        connectButton.setText(R.string.connect_to_device);
                        connectProgress.setVisibility(View.GONE);
                    }
                }

                @Override
                public void onScanStarted(boolean success) {

                }

                @Override
                public void onScanning(BleDevice bleDevice) {
                    String name = bleDevice.getName();
                    if (name != null && name.equals(BLE_NAME)) {
                        SignBuddy = bleDevice;
                        BleManager.getInstance().connect(SignBuddy, new BleGattCallback() {
                            @Override
                            public void onStartConnect() {
                                connectButton.setText(R.string.connecting);
                            }

                            @Override
                            public void onConnectFail(BleDevice bleDevice, BleException exception) {
                                Toast.makeText(MainActivity.this, "Connection failed!", Toast.LENGTH_SHORT).show();
                                connectButton.setEnabled(true);
                                connectButton.setText(R.string.connect_to_device);
                                connectProgress.setVisibility(View.GONE);
                            }

                            @Override
                            public void onConnectSuccess(BleDevice bleDevice, BluetoothGatt gatt, int status) {
                                Toast.makeText(MainActivity.this, "Connected!", Toast.LENGTH_SHORT).show();
                                connectButton.setText(R.string.connected);
                                connectProgress.setVisibility(View.GONE);
                                resetButton.setVisibility(View.VISIBLE);
                                BleManager.getInstance().notify(SignBuddy, UUID_SERVICE, UUID_CHARACTERISTIC_NOTIFY, new BleNotifyCallback() {
                                    @Override
                                    public void onNotifySuccess() {

                                    }

                                    @Override
                                    public void onNotifyFailure(BleException exception) {

                                    }

                                    @Override
                                    public void onCharacteristicChanged(byte[] data) {
                                        for (byte b : data) {
                                            if (b == MSG_SYNC && byteBuffer.size() == 0) {
                                                skip_crc = 4;
                                                next_byte_is_length = true;
                                            } else if (next_byte_is_length) {
                                                msg_length = b;
                                                next_byte_is_length = false;
                                            } else if (skip_crc > 0) {
                                                skip_crc--;
                                            } else {
                                                byteBuffer.write(b);
                                                if (byteBuffer.size() == msg_length) {
                                                    new Thread(new ParseMessage(byteBuffer.toByteArray())).start();
                                                    byteBuffer.reset();
                                                }
                                            }
                                        }
                                    }
                                });
                            }

                            @Override
                            public void onDisConnected(boolean isActiveDisConnected, BleDevice device, BluetoothGatt gatt, int status) {
                                Toast.makeText(MainActivity.this, "Disconnected!", Toast.LENGTH_SHORT).show();
                                connectButton.setEnabled(true);
                                connectButton.setText(R.string.connect_to_device);
                                connectProgress.setVisibility(View.GONE);
                                resetButton.setVisibility(View.GONE);
                            }
                        });
                    }
                }
            });
        }
    }

    private void resetButtonCallback(View v) {
        BleManager.getInstance().write(SignBuddy, UUID_SERVICE, UUID_CHARACTERISTIC_WRITE, CMD_RESET_IMU, new BleWriteCallback() {
            @Override
            public void onWriteSuccess(int current, int total, byte[] justWrite) {
                Toast.makeText(MainActivity.this, "Reset IMU!", Toast.LENGTH_SHORT).show();
            }

            @Override
            public void onWriteFailure(BleException exception) {
                Toast.makeText(MainActivity.this, "Failed to reset IMU!", Toast.LENGTH_SHORT).show();
            }
        });
    }

    private void requestFineLocation() {
        if (ActivityCompat.shouldShowRequestPermissionRationale(this, Manifest.permission.ACCESS_FINE_LOCATION)) {
            new AlertDialog.Builder(this)
                    .setTitle("Permission needed")
                    .setMessage("This permission is needed for BLE scanning")
                    .setPositiveButton("Ok", (dialogInterface, i) -> ActivityCompat.requestPermissions(MainActivity.this, new String[]{Manifest.permission.ACCESS_FINE_LOCATION}, FINE_LOCATION_CODE))
                    .setNegativeButton("Cancel", (dialogInterface, i) -> dialogInterface.dismiss())
                    .create().show();
        } else {
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.ACCESS_FINE_LOCATION}, FINE_LOCATION_CODE);
        }
    }

    @SuppressLint("MissingSuperCall")
    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        if (requestCode == FINE_LOCATION_CODE) {
            if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                Toast.makeText(this, "Permission GRANTED", Toast.LENGTH_SHORT).show();
                connectButton.setVisibility(View.VISIBLE);
            } else {
                Toast.makeText(this, "Permission DENIED", Toast.LENGTH_SHORT).show();
            }
        }
    }

    private class ParseMessage implements Runnable {

        private byte[] msg;

        public ParseMessage(byte[] msg) {
            this.msg = msg;
        }

        public void run() {
            try {
                SignBuddyProto.SBPMessage message = SignBuddyProto.SBPMessage.parseFrom(this.msg);
                if (message.getId() == MID_SAMPLE) {
                    SignBuddyProto.SBPSample sample = message.getSample();
                    Log.i("Sign Buddy", "NEW SAMPLE");
                    samplesLock.lock();
                    samples.add(sample);
                    Log.i("Sign Buddy", String.valueOf(samples.size()));
                    if (samples.size() == num_samples) {
                        SignBuddyProto.SBPGestureData gestureData = SignBuddyProto.SBPGestureData.newBuilder()
                                .setLetter(gestureLetter)
                                .addAllSamples(samples)
                                .build();
                        Log.i("SignBuddy", gestureData.toString());

                        File file = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS);

                        file = new File(file, "sign_buddy_training_data.txt");

                        if (!file.exists()) {
                            file.createNewFile();
                        }

                        PrintWriter writer = new PrintWriter(new OutputStreamWriter(new FileOutputStream(file, true)));
                        writer.println(gestureData);
                        writer.println("####################################################");
                        writer.println("####################################################");
                        writer.println("####################################################");

                        writer.flush();
                        writer.close();

                        samples.clear();
                        runOnUiThread(() -> {
                            Toast.makeText(MainActivity.this, "Gesture collected!", Toast.LENGTH_SHORT).show();
                            collectButton.setEnabled(true);
                            collectButton.setText(R.string.collect);
                        });
                        Log.i("Sign Buddy", "NEW GESTURE");
                    }
                    samplesLock.unlock();
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
}
