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
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.Toast;

import com.clj.fastble.BleManager;
import com.clj.fastble.callback.BleGattCallback;
import com.clj.fastble.callback.BleNotifyCallback;
import com.clj.fastble.callback.BleScanCallback;
import com.clj.fastble.data.BleDevice;
import com.clj.fastble.exception.BleException;
import com.clj.fastble.scan.BleScanRuleConfig;

import java.util.List;

public class MainActivity extends AppCompatActivity {

    private Button connectButton;
    private ProgressBar connectProgress;
    private final int FINE_LOCATION_CODE = 1;
    private BleDevice SignBuddy;
    private final String ble_name = "Sign Buddy BLE";
    private final String uuid_service = "6E400001-B5A3-F393-E0A9-E50E24DCCA9E";
    private final String uuid_characteristic_notify = "6E400003-B5A3-F393-E0A9-E50E24DCCA9E";
    private int byteCount = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button lessonButton = (Button) findViewById(R.id.lessonButton);
        lessonButton.setOnClickListener(this::openLessonActivity);

        Button quizButton = (Button) findViewById(R.id.quizButton);
        quizButton.setOnClickListener(this::openQuizActivity);

        connectProgress = findViewById(R.id.connectProgress);
        connectButton = findViewById(R.id.connectButton);
        connectButton.setOnClickListener(this::connectButtonCallback);

        BleManager.getInstance().init(getApplication());
        BleManager.getInstance()
                .enableLog(true)
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
        Intent intent = new Intent(this, LessonActivity.class);
        startActivity(intent);
    }

    private void openQuizActivity(View v) {
        Intent intent = new Intent(this, QuizActivity.class);
        startActivity(intent);
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
                    if (name != null && name.equals(ble_name)) {
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
                                BleManager.getInstance().notify(SignBuddy, uuid_service, uuid_characteristic_notify, new BleNotifyCallback() {
                                    @Override
                                    public void onNotifySuccess() {

                                    }

                                    @Override
                                    public void onNotifyFailure(BleException exception) {

                                    }

                                    @Override
                                    public void onCharacteristicChanged(byte[] data) {
                                        StringBuilder sb = new StringBuilder();
                                        for (byte b : data) {
                                            sb.append(String.format("%02X ", b));
                                            byteCount++;
                                        }
                                        Log.i("Sign Buddy", sb.toString());
                                        Log.i("Sign Buddy", String.valueOf(byteCount));
                                    }
                                });
                            }

                            @Override
                            public void onDisConnected(boolean isActiveDisConnected, BleDevice device, BluetoothGatt gatt, int status) {
                                Toast.makeText(MainActivity.this, "Disconnected!", Toast.LENGTH_SHORT).show();
                                connectButton.setEnabled(true);
                                connectButton.setText(R.string.connect_to_device);
                                connectProgress.setVisibility(View.GONE);
                            }
                        });
                    }
                }
            });
        }
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
}
