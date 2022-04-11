package com.example.signbuddy;

import androidx.appcompat.app.AppCompatActivity;
import android.content.Intent;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.Environment;
import android.util.Log;
import android.view.View;
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

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.text.BreakIterator;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.locks.ReentrantLock;


public class LessonActivity extends AppCompatActivity {
    private Button correctButton;
    private Button incorrectButton;
    private ProgressBar pgsBar;
    private TextView countText;
    int alpha; //coming from previous activity (1-5)
    int SVCalpha;//outputted by SVC
    int lessonnum; //coming from previous activity
    int alphanum; //mapped to actual alpha number (1-26)
    boolean correct;
    private BleDevice SignBuddy;
    private ReentrantLock samplesLock = new ReentrantLock();
    private List<SignBuddyProto.SBPSample> samples = new ArrayList<>();
    private ByteArrayOutputStream byteBuffer = new ByteArrayOutputStream();
    private final byte[] CMD_SAMPLE_ONCE = {0x01};

    private final String UUID_SERVICE = "6E400001-B5A3-F393-E0A9-E50E24DCCA9E";
    private final String UUID_CHARACTERISTIC_WRITE = "6E400002-B5A3-F393-E0A9-E50E24DCCA9E";
    private final String UUID_CHARACTERISTIC_NOTIFY = "6E400003-B5A3-F393-E0A9-E50E24DCCA9E";
    private final byte MSG_SYNC = 0x16;
    private SignBuddyProto.SBPGestureData gestureData;
    private final int MAX_SAMPLES = 40;
    private final byte MID_SAMPLE = 0x01;
    private int num_samples;
    private int msg_length;
    private int skip_crc;
    private boolean next_byte_is_length;
    private String gestureLetter = "";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_alesson);
        ImageView image = (ImageView) findViewById(R.id.signImage);

        //setting gesture image
        Intent i = getIntent();
        String lessonNumber = i.getStringExtra("LessonNumber");
        lessonnum = Integer.valueOf(lessonNumber);
        alpha = i.getIntExtra("alpha", 0);

        //set Progress Bar
        pgsBar = (ProgressBar) findViewById(R.id.progressBar);
        pgsBar.setProgress(alpha);

        //setting current letter
        if (alpha ==5){
            finish();
            Toast.makeText(LessonActivity.this, "You have completed Lesson " + lessonnum, Toast.LENGTH_SHORT).show();
            Intent intent = new Intent(LessonActivity.this, LessonMenuActivity.class);
            startActivity(intent);

        }

        //setting current image
        alphanum  = (lessonnum-1)*5+alpha;
        String alphaNumber = String.valueOf(alphanum);
        String fnm = "alpha_"+alphaNumber; //  image file name
        String PACKAGE_NAME = getApplicationContext().getPackageName();
        int imgId = getResources().getIdentifier(PACKAGE_NAME+":drawable/"+fnm , null, null);
        image.setImageResource(imgId);

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
        new CountDownTimer(7500, 1500) {
            public void onTick(long millisUntilFinished) {
                countText = findViewById(R.id.countText);
                countText.setText("Perform Sign in: " + ((millisUntilFinished / 1500)+1));
            }
            public void onFinish() {
                byte[] cmd;
                cmd = CMD_SAMPLE_ONCE;
                BleManager.getInstance().write(SignBuddy, UUID_SERVICE, UUID_CHARACTERISTIC_WRITE, cmd, new BleWriteCallback() {
                    @Override
                    public void onWriteSuccess(int current, int total, byte[] justWrite) {
                        Toast.makeText(LessonActivity.this, "Starting collection!", Toast.LENGTH_SHORT).show();
                    }

                    @Override
                    public void onWriteFailure(BleException exception) {
                        Toast.makeText(LessonActivity.this, "Failed to start collection!", Toast.LENGTH_SHORT).show();
                    }
                });
                countText.setText("GO!");
                try {
                    Thread.sleep(500);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                countText.setText(gestureData.toString());

            }
        }.start();

        if (checkCorrect()==true) {
            Toast.makeText(LessonActivity.this, "Correct!", Toast.LENGTH_SHORT).show();
            alpha++;
            finish();
            i.addFlags(Intent.FLAG_ACTIVITY_NO_ANIMATION);
            Bundle mbundle = new Bundle();
            mbundle.putInt("alpha", alpha);
            mbundle.putString("LessonNumber", lessonNumber);
            i.putExtras(mbundle);

            startActivity(i);
            overridePendingTransition(0, 0);
        }
        else {
            Toast.makeText(LessonActivity.this, "Incorrect, Please try again!", Toast.LENGTH_SHORT).show();
        }


    }

    private void runSVC() {

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
                        gestureData = SignBuddyProto.SBPGestureData.newBuilder()
                                .addAllSamples(samples)
                                .build();
                        Log.i("SignBuddy", gestureData.toString());

                        samples.clear();
                        runOnUiThread(() -> {
                            Toast.makeText(LessonActivity.this, "Gesture collected!", Toast.LENGTH_SHORT).show();
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

    private void callSVC() {
        String data0 = String.valueOf((gestureData.getSamples(0).getFlexData().getFlexIndex())/641.82241);
        String data1 = String.valueOf((gestureData.getSamples(0).getFlexData().getFlexLittle())/838.81047);
        String data2 = String.valueOf((gestureData.getSamples(0).getFlexData().getFlexMiddle())/798.91990);
        String data3 = String.valueOf((gestureData.getSamples(0).getFlexData().getFlexRing())/935.76600);
        String data4 = String.valueOf((gestureData.getSamples(0).getFlexData().getFlexThumb())/1010.25888);
        String data5 = String.valueOf((gestureData.getSamples(0).getImuData().getQuatW())/89806.69545);
        String data6 = String.valueOf((gestureData.getSamples(0).getImuData().getQuatX())/89806.69545);
        String data7 = String.valueOf((gestureData.getSamples(0).getImuData().getQuatY())/70859.62009);
        String data8 = String.valueOf((gestureData.getSamples(0).getImuData().getQuatZ())/124341.31934);
        String data9 = String.valueOf((gestureData.getSamples(0).getTouchData().getTouch1() ? 1 : -1)/12.64911);
        String data10 = String.valueOf((gestureData.getSamples(0).getTouchData().getTouch10() ? 1 : -1)/12.64911);
        String data11 = String.valueOf((gestureData.getSamples(0).getTouchData().getTouch11() ? 1 : -1)/12.64911);
        String data12 = String.valueOf((gestureData.getSamples(0).getTouchData().getTouch12() ? 1 : -1)/12.64911);
        String data13 = String.valueOf((gestureData.getSamples(0).getTouchData().getTouch2() ? 1 : -1)/12.64911);
        String data14 = String.valueOf((gestureData.getSamples(0).getTouchData().getTouch3() ? 1 : -1)/12.64911);
        String data15 = String.valueOf((gestureData.getSamples(0).getTouchData().getTouch4() ? 1 : -1)/12.64911);
        String data16 = String.valueOf((gestureData.getSamples(0).getTouchData().getTouch5() ? 1 : -1)/12.64911);
        String data17 = String.valueOf((gestureData.getSamples(0).getTouchData().getTouch6() ? 1 : -1)/12.64911);
        String data18 = String.valueOf((gestureData.getSamples(0).getTouchData().getTouch7() ? 1 : -1)/12.64911);
        String data19 = String.valueOf((gestureData.getSamples(0).getTouchData().getTouch8() ? 1 : -1)/12.64911);
        String data20 = String.valueOf((gestureData.getSamples(0).getTouchData().getTouch9() ? 1 : -1)/12.64911);
        String[]  args = {data0, data1, data2, data3, data4, data5, data6, data7, data8, data9, data10, data11, data12, data13, data14, data15, data16, data17, data18, data19, data20};
        SVCalpha = SVC.main(args)+1;

    }
    private boolean checkCorrect() {

        if (alphanum<=9){
            correct = (alphanum==SVCalpha);
        }
        //skip j
        if (alphanum<26 && alphanum>10){
            correct = (alphanum==SVCalpha-1);
        }
        if (alphanum==10 || alphanum ==26) {
            correct = true;//skip this letter
        }
        return correct;
    }

}