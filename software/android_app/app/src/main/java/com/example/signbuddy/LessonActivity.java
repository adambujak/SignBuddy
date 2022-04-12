package com.example.signbuddy;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.util.Log;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import com.clj.fastble.BleManager;
import com.clj.fastble.callback.BleNotifyCallback;
import com.clj.fastble.callback.BleWriteCallback;
import com.clj.fastble.data.BleDevice;
import com.clj.fastble.exception.BleException;
import com.clj.fastble.scan.BleScanRuleConfig;

import java.io.ByteArrayOutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.locks.ReentrantLock;


public class LessonActivity extends AppCompatActivity {
    private ProgressBar pgsBar;
    private TextView countText;
    int alpha; //coming from previous activity (1-5)
    int SVCalpha;//outputted by SVC
    int lessonnum; //coming from previous activity
    int alphanum; //mapped to actual alpha number (1-26)
    boolean correct;
    private BleDevice SignBuddy;
    private ByteArrayOutputStream byteBuffer = new ByteArrayOutputStream();
    private final byte[] CMD_SAMPLE_ONCE = {0x01};

    private final String UUID_SERVICE = "6E400001-B5A3-F393-E0A9-E50E24DCCA9E";
    private final String UUID_CHARACTERISTIC_WRITE = "6E400002-B5A3-F393-E0A9-E50E24DCCA9E";
    private final String UUID_CHARACTERISTIC_NOTIFY = "6E400003-B5A3-F393-E0A9-E50E24DCCA9E";
    private final byte MSG_SYNC = 0x16;
    private SignBuddyProto.SBPGestureData gestureData;
    private final byte MID_SAMPLE = 0x01;
    private final int num_samples = 1;
    private int msg_length;
    private int skip_crc;
    private boolean next_byte_is_length;
    private String gestureLetter = "";
    private Intent i;
    private String lessonNumber;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_alesson);
        SignBuddy = BleManager.getInstance().getAllConnectedDevice().get(0);
        ImageView image = (ImageView) findViewById(R.id.signImage);

        //setting gesture image
        i = getIntent();
        lessonNumber = i.getStringExtra("LessonNumber");
        lessonnum = Integer.valueOf(lessonNumber);
        alpha = i.getIntExtra("alpha", 0);

        //set Progress Bar
        pgsBar = (ProgressBar) findViewById(R.id.progressBar);
        pgsBar.setProgress(alpha);

        //setting current letter
        if (alpha == 5) {
            finish();
            Toast.makeText(LessonActivity.this, "You have completed Lesson " + lessonnum, Toast.LENGTH_SHORT).show();
            Intent intent = new Intent(LessonActivity.this, LessonMenuActivity.class);
            startActivity(intent);

        }

        //setting current image
        alphanum = (lessonnum - 1) * 5 + alpha;
        String alphaNumber = String.valueOf(alphanum);
        String fnm = "alpha_" + alphaNumber; //  image file name
        String PACKAGE_NAME = getApplicationContext().getPackageName();
        int imgId = getResources().getIdentifier(PACKAGE_NAME + ":drawable/" + fnm, null, null);
        image.setImageResource(imgId);
        BleManager.getInstance().init(getApplication());
        BleManager.getInstance()
                .enableLog(false)
                .setReConnectCount(1, 5000)
                .setOperateTimeout(5000);

        BleScanRuleConfig scanRuleConfig = new BleScanRuleConfig.Builder()
                .setScanTimeOut(3000)
                .build();

        BleManager.getInstance().initScanRule(scanRuleConfig);


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
                            new Thread(new LessonActivity.ParseMessage(byteBuffer.toByteArray())).start();
                            byteBuffer.reset();
                        }
                    }
                }
            }
        });
        //start countdown
        new CountDownTimer(5000, 1000) {
            public void onTick(long millisUntilFinished) {
                countText = findViewById(R.id.countText);
                countText.setText("Perform Sign in: " + ((millisUntilFinished / 1000) + 1));
            }

            public void onFinish() {
                byte[] cmd;
                cmd = CMD_SAMPLE_ONCE;
                BleManager.getInstance().write(SignBuddy, UUID_SERVICE, UUID_CHARACTERISTIC_WRITE, cmd, new BleWriteCallback() {
                    @Override
                    public void onWriteSuccess(int current, int total, byte[] justWrite) {
                    }

                    @Override
                    public void onWriteFailure(BleException exception) {
                        Toast.makeText(LessonActivity.this, "Failed to start collection!", Toast.LENGTH_SHORT).show();
                    }
                });
                countText.setText("GO!");
            }
        }.start();
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
                    List<SignBuddyProto.SBPSample> samples = new ArrayList<>();
                    samples.add(sample);
                    gestureData = SignBuddyProto.SBPGestureData.newBuilder()
                            .setLetter(gestureLetter)
                            .addAllSamples(samples)
                            .build();
                    Log.i("SignBuddy", gestureData.toString());

                    samples.clear();
                    Log.i("Sign Buddy", "NEW GESTURE");
                    if (checkCorrect()) {
                        runOnUiThread(() -> {
                            Toast.makeText(LessonActivity.this, "Correct!", Toast.LENGTH_SHORT).show();
                        });
                        alpha++;
                        finish();
                        i.addFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
                        Bundle mbundle = new Bundle();
                        mbundle.putInt("alpha", alpha);
                        mbundle.putString("LessonNumber", lessonNumber);
                        i.putExtras(mbundle);

                        startActivity(i);
                        overridePendingTransition(0, 0);
                    } else {
                        runOnUiThread(() -> {
                            Toast.makeText(LessonActivity.this, "Incorrect, Please try again!", Toast.LENGTH_SHORT).show();
                            i.addFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
                            recreate();
                        });
                    }
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

    }

    private void callSVC() {
        String data0 = String.valueOf((gestureData.getSamples(0).getFlexData().getFlexIndex()) / 756.37358);
        String data1 = String.valueOf((gestureData.getSamples(0).getFlexData().getFlexLittle()) / 826.82646);
        String data2 = String.valueOf((gestureData.getSamples(0).getFlexData().getFlexMiddle()) / 929.51170);
        String data3 = String.valueOf((gestureData.getSamples(0).getFlexData().getFlexRing()) / 965.84108);
        String data4 = String.valueOf((gestureData.getSamples(0).getFlexData().getFlexThumb()) / 1011.90266);
        String data5 = String.valueOf((gestureData.getSamples(0).getImuData().getQuatW()) / 79198.56836);
        String data6 = String.valueOf((gestureData.getSamples(0).getImuData().getQuatX()) / 79198.56836);
        String data7 = String.valueOf((gestureData.getSamples(0).getImuData().getQuatY()) / 59061.66665);
        String data8 = String.valueOf((gestureData.getSamples(0).getImuData().getQuatZ()) / 140609.55664);
        String data9 = String.valueOf((gestureData.getSamples(0).getTouchData().getTouch1() ? 1 : -1) / 12.60952);
        String data10 = String.valueOf((gestureData.getSamples(0).getTouchData().getTouch10() ? 1 : -1) / 12.60952);
        String data11 = String.valueOf((gestureData.getSamples(0).getTouchData().getTouch11() ? 1 : -1) / 12.60952);
        String data12 = String.valueOf((gestureData.getSamples(0).getTouchData().getTouch12() ? 1 : -1) / 12.60952);
        String data13 = String.valueOf((gestureData.getSamples(0).getTouchData().getTouch2() ? 1 : -1) / 12.60952);
        String data14 = String.valueOf((gestureData.getSamples(0).getTouchData().getTouch3() ? 1 : -1) / 12.60952);
        String data15 = String.valueOf((gestureData.getSamples(0).getTouchData().getTouch4() ? 1 : -1) / 12.60952);
        String data16 = String.valueOf((gestureData.getSamples(0).getTouchData().getTouch5() ? 1 : -1) / 12.60952);
        String data17 = String.valueOf((gestureData.getSamples(0).getTouchData().getTouch6() ? 1 : -1) / 12.60952);
        String data18 = String.valueOf((gestureData.getSamples(0).getTouchData().getTouch7() ? 1 : -1) / 12.60952);
        String data19 = String.valueOf((gestureData.getSamples(0).getTouchData().getTouch8() ? 1 : -1) / 12.60952);
        String data20 = String.valueOf((gestureData.getSamples(0).getTouchData().getTouch9() ? 1 : -1) / 12.60952);
        String[] args = {data0, data1, data2, data3, data4, data5, data6, data7, data8, data9, data10, data11, data12, data13, data14, data15, data16, data17, data18, data19, data20};
        SVCalpha = SVC.main(args)+1;

    }

    private boolean checkCorrect() {
        callSVC();
        if (alphanum <= 9) {
            correct = (alphanum == SVCalpha);
        }
        //skip j
        if (alphanum < 26 && alphanum > 10) {
            correct = (alphanum == SVCalpha - 1);
        }
        if (alphanum == 10 || alphanum == 26) {
            correct = true;//skip this letter
        }
        return correct;
    }

}