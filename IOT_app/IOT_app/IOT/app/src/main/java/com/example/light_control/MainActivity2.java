package com.example.light_control;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.NotificationCompat;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.os.Build;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class MainActivity2 extends AppCompatActivity {
    private TextView nhietdo;
    private TextView mq2TextView;
    private TextView humidityTextView;
    private TextView WaterTextView;
    DatabaseReference nhietdodata;
    DatabaseReference gasdata;
    DatabaseReference humiditydata;
    DatabaseReference watersensordata;
    private TextView phantramden1;
    private TextView phantramden2;
    private TextView phantramden3;
    @SuppressLint("UseSwitchCompatOrMaterialCode")
    private ImageButton swden1;
    @SuppressLint("UseSwitchCompatOrMaterialCode")
    private ImageButton swden2;
    @SuppressLint("UseSwitchCompatOrMaterialCode")
    private ImageButton swden3;
    private SeekBar thanhden1;
    private SeekBar thanhden2;
    private SeekBar thanhden3;
    boolean sw1data_status;
    boolean sw2data_status;
    boolean sw3data_status;
    boolean isFirebaseUpdate = false;
    boolean isFirebaseUpdate1 = false;
    boolean isFirebaseUpdate2 = false;
    boolean isFirebaseUpdate3 = false;
    private boolean[] isOn = {false,false, false, false,false, false,false};
    private DatabaseReference den1_data;
    private DatabaseReference den2_data;
    private DatabaseReference den3_data;
    private DatabaseReference sw1data;
    private DatabaseReference sw2data;
    private DatabaseReference sw3data;
    private Button btnlogout;
    private Button dingu;
    private Button thucday;
    private Button rangoai;
    private Button tietkiemnangluong;
    private DatabaseReference databaseRef_dingu;
    private DatabaseReference databaseRef_tietkiemnangluong;
    private DatabaseReference databaseRef_thucday;
    private DatabaseReference databaseRef_rangoai;
    private int dosang;
    ImageButton btnToclock;
    ImageButton btnToRGB;
    ImageButton btnTocamera;
    public MainActivity2() {
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main2);
        btnlogout = findViewById(R.id.btnlogout);
        tietkiemnangluong = findViewById(R.id.tietkiemnangluong);
        dingu = findViewById(R.id.dingu);
        thucday = findViewById(R.id.thucday);
        rangoai = findViewById(R.id.dilam);
        phantramden1=findViewById(R.id.phantramden1);
        phantramden2=findViewById(R.id.phantramden2);
        phantramden3=findViewById(R.id.phantramden3);
        swden1=findViewById(R.id.swden1);
        swden2=findViewById(R.id.swden2);
        swden3=findViewById(R.id.swden3);
        thanhden1=findViewById(R.id.thanhden1);
        thanhden2=findViewById(R.id.thanhden2);
        thanhden3=findViewById(R.id.thanhden3);
        nhietdo=findViewById(R.id.nhietdo);
        mq2TextView = findViewById(R.id.mq2TextView);
        humidityTextView = findViewById(R.id.humidityTextView);
        WaterTextView = findViewById(R.id.WaterTextView);
        btnToRGB = findViewById(R.id.btnToRGB);
        btnToclock = findViewById(R.id.btnToclock);
        btnTocamera = findViewById(R.id.btncamera);
        sw1data=FirebaseDatabase.getInstance().getReference().child("BT1");
        sw2data=FirebaseDatabase.getInstance().getReference().child("BT2");
        sw3data=FirebaseDatabase.getInstance().getReference().child("BT3");
        databaseRef_dingu = FirebaseDatabase.getInstance().getReference("DI_NGU");
        databaseRef_tietkiemnangluong = FirebaseDatabase.getInstance().getReference("TIET_KIEM_NANG_LUONG");
        databaseRef_thucday = FirebaseDatabase.getInstance().getReference("THUC_DAY");
        databaseRef_rangoai = FirebaseDatabase.getInstance().getReference("RA_NGOAI");
        //nhom12@gmail.com
        //1234567
        // Xử lý đồng bộ nút switch 1
        sw1data.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                if (snapshot.exists()) {
                    isFirebaseUpdate1 = true;
                    sw1data_status = snapshot.getValue(Boolean.class);
                    isOn[0] = (sw1data_status == true);
                    swden1.setSelected(isOn[0]);
                }else{
                    isOn[0] = false;
                    swden1.setSelected(isOn[0]);
                }
            }
            public void onCancelled(@NonNull DatabaseError error) {
                //NOP
            }
        });
        // Xử lý nút switch 1
        swden1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (isOn[0]) {
                    swden1.setSelected(isOn[0]);
                    sw1data.setValue(false);
                    isOn[0] = false;
                } else {
                    swden1.setSelected(isOn[0]);
                    sw1data.setValue(true);
                    isOn[0] = true;
                }
            }
        });
        //Xử lý đồng bộ nút switch 2
        sw2data.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                if (snapshot.exists()) {
                    isFirebaseUpdate2 = true;
                    sw2data_status = snapshot.getValue(Boolean.class);
                    isOn[1] = ( sw2data_status == true);
                    swden2.setSelected(isOn[1]);
                }else {
                    isOn[1] = false;
                    swden2.setSelected(isOn[1]);
                }
            }
            public void onCancelled(@NonNull DatabaseError error) {
                //NOP
            }
        });
        //Xử lý nút switch 2
        swden2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (isOn[1]) {
                    swden2.setSelected(isOn[1]);
                    sw2data.setValue(false);
                    isOn[1] = false;
                } else {
                    swden2.setSelected(isOn[1]);
                    sw2data.setValue(true);
                    isOn[1] = true;
                }
            }
        });
        //Xử lý nút switch 3
        swden3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (isOn[6]) {
                    swden3.setSelected(isOn[6]);
                    sw3data.setValue(false);
                    isOn[6] = false;
                } else {
                    swden3.setSelected(isOn[6]);
                    sw3data.setValue(true);
                    isOn[6] = true;
                }
            }
        });
        //Xử lý đồng bộ nút switch 3
        sw3data.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                if (snapshot.exists()) {
                    isFirebaseUpdate3 = true;
                    sw3data_status = snapshot.getValue(Boolean.class);
                    isOn[6] = ( sw3data_status == true);
                    swden3.setSelected(isOn[6]);
                }else {
                    isOn[6] = false;
                    swden3.setSelected(isOn[6]);
                }
            }
            public void onCancelled(@NonNull DatabaseError error) {
                //NOP
            }
        });
        //Xử lí nút chuyển sang cài đặt RGB
        btnToRGB.setOnClickListener(v -> {
            Intent intent = new Intent(MainActivity2.this, MainActivity3.class);
            startActivity(intent);
        });
        //Xử lí nt chuyển sang cài đặt hẹn giờ
        btnToclock.setOnClickListener(v -> {
            Intent intent = new Intent(MainActivity2.this, MainActivity4.class);
            startActivity(intent);
        });
        //Xử lí nt chuyển sang giao diện xem camera
        btnTocamera.setOnClickListener(v -> {
            Intent intent = new Intent(MainActivity2.this, MainActivity5.class);
            startActivity(intent);
        });
        // trượt thanh ngang đèn 1
        den1_data=FirebaseDatabase.getInstance().getReference().child("LB1");
        den1_data.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                if (snapshot.exists()) {
                    dosang = snapshot.getValue(Integer.class);
                    isFirebaseUpdate = true;
                    thanhden1.setProgress(dosang);
                    phantramden1.setText(dosang + "%");
                }
            }
            public void onCancelled(@NonNull DatabaseError error) {
                //NOP
            }
        });
        thanhden1.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                if(!isFirebaseUpdate){
                    den1_data.setValue(i);
                    phantramden1.setText(i + "%");
                }
                    isFirebaseUpdate = false;
            }
//                if(i<10){
//                    den1_data.child("MUC SANG DEN 1").setValue(0);
//                    dosang=0;
//                    phantramden1.setText(dosang+"%");
//                }else if(i>=10 && i<20){
//                    den1_data.child("MUC SANG DEN 1").setValue(10);
//                    dosang=10;
//                    phantramden1.setText(dosang+"%");
//                }else if(i>10 && i<20){
//                    den1_data.child("MUC SANG DEN 1").setValue(10);
//                    dosang=10;
//                    phantramden1.setText(dosang+"%");
//                }else if(i>=20 && i<30){
//                    den1_data.child("MUC SANG DEN 1").setValue(20);
//                    dosang=20;
//                    phantramden1.setText(dosang+"%");
//                }else if(i>=30 && i<40){
//                    den1_data.child("MUC SANG DEN 1").setValue(30);
//                    dosang=30;
//                    phantramden1.setText(dosang+"%");
//                }else if(i>=40 && i<50){
//                    den1_data.child("MUC SANG DEN 1").setValue(40);
//                    dosang=40;
//                    phantramden1.setText(dosang+"%");
//                }else if(i>=50 && i<60){
//                    den1_data.child("MUC SANG DEN 1").setValue(50);
//                    dosang=50;
//                    phantramden1.setText(dosang+"%");
//                }else if(i>=60 && i<70){
//                    den1_data.child("MUC SANG DEN 1").setValue(60);
//                    dosang=60;
//                    phantramden1.setText(dosang+"%");
//                }else if(i>=70 && i<80){
//                    den1_data.child("MUC SANG DEN 1").setValue(70);
//                    dosang=70;
//                    phantramden1.setText(dosang+"%");
//                }else if(i>=80 && i<90){
//                    den1_data.child("MUC SANG DEN 1").setValue(80);
//                    dosang=80;
//                    phantramden1.setText(dosang+"%");
//                }else if(i>=90 && i<100){
//                    den1_data.child("MUC SANG DEN 1").setValue(90);
//                    dosang=90;
//                    phantramden1.setText(dosang+"%");
//                }else {
//                    den1_data.child("MUC SANG DEN 1").setValue(100);
//                    dosang=100;
//                    phantramden1.setText(dosang+"%");
//                }///////////
            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }
            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
            }
        });
        // trượt thanh ngang đèn 2
        den2_data=FirebaseDatabase.getInstance().getReference().child("LB2");
        den2_data.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                if (snapshot.exists()) {
                    dosang = snapshot.getValue(Integer.class);
                    isFirebaseUpdate = true;
                    thanhden2.setProgress(dosang);
                    phantramden2.setText(dosang + "%");
                }
            }
            public void onCancelled(@NonNull DatabaseError error) {
                //NOP
            }
        });

        thanhden2.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b){
                if (!isFirebaseUpdate) {
                    den2_data.setValue(i);
                    phantramden2.setText(i + "%");
                }
                isFirebaseUpdate = false;
                }
//                if(i<10){
//                    den2_data.child("MUC SANG DEN 2").setValue(0);
//                    dosang=0;
//                    phantramden2.setText(dosang+"%");
//                }else if(i>=10 && i<20){
//                    den2_data.child("MUC SANG DEN 2").setValue(10);
//                    dosang=10;
//                    phantramden2.setText(dosang+"%");
//                }else if(i>10 && i<20){
//                    den2_data.child("MUC SANG DEN 2").setValue(10);
//                    dosang=10;
//                    phantramden2.setText(dosang+"%");
//                }else if(i>=20 && i<30){
//                    den2_data.child("MUC SANG DEN 2").setValue(20);
//                    dosang=20;
//                    phantramden2.setText(dosang+"%");
//                }else if(i>=30 && i<40){
//                    den2_data.child("MUC SANG DEN 2").setValue(30);
//                    dosang=30;
//                    phantramden2.setText(dosang+"%");
//                }else if(i>=40 && i<50){
//                    den2_data.child("MUC SANG DEN 2").setValue(40);
//                    dosang=40;
//                    phantramden2.setText(dosang+"%");
//                }else if(i>=50 && i<60){
//                    den2_data.child("MUC SANG DEN 2").setValue(50);
//                    dosang=50;
//                    phantramden2.setText(dosang+"%");
//                }else if(i>=60 && i<70){
//                    den2_data.child("MUC SANG DEN 2").setValue(60);
//                    dosang=60;
//                    phantramden2.setText(dosang+"%");
//                }else if(i>=70 && i<80){
//                    den2_data.child("MUC SANG DEN 2").setValue(70);
//                    dosang=70;
//                    phantramden2.setText(dosang+"%");
//                }else if(i>=80 && i<90){
//                    den2_data.child("MUC SANG DEN 2").setValue(80);
//                    dosang=80;
//                    phantramden2.setText(dosang+"%");
//                }else if(i>=90 && i<100){
//                    den2_data.child("MUC SANG DEN 2").setValue(90);
//                    dosang=90;
//                    phantramden2.setText(dosang+"%");
//                }else {
//                    den2_data.child("MUC SANG DEN 2").setValue(100);
//                    dosang=100;
//                    phantramden2.setText(dosang+"%");
//                }////////////////
            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }
            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
            }
        });
        // trượt thanh ngang đèn 2
        den3_data=FirebaseDatabase.getInstance().getReference().child("LB3");
        den3_data.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                if (snapshot.exists()) {
                    dosang = snapshot.getValue(Integer.class);
                    isFirebaseUpdate = true;
                    thanhden3.setProgress(dosang);
                    phantramden3.setText(dosang + "%");
                }
            }
            public void onCancelled(@NonNull DatabaseError error) {
                //NOP
            }
        });

        thanhden3.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b){
                if (!isFirebaseUpdate) {
                    den3_data.setValue(i);
                    phantramden3.setText(i + "%");
                }
                isFirebaseUpdate = false;
            }
            //                if(i<10){
//                    den2_data.child("MUC SANG DEN 2").setValue(0);
//                    dosang=0;
//                    phantramden2.setText(dosang+"%");
//                }else if(i>=10 && i<20){
//                    den2_data.child("MUC SANG DEN 2").setValue(10);
//                    dosang=10;
//                    phantramden2.setText(dosang+"%");
//                }else if(i>10 && i<20){
//                    den2_data.child("MUC SANG DEN 2").setValue(10);
//                    dosang=10;
//                    phantramden2.setText(dosang+"%");
//                }else if(i>=20 && i<30){
//                    den2_data.child("MUC SANG DEN 2").setValue(20);
//                    dosang=20;
//                    phantramden2.setText(dosang+"%");
//                }else if(i>=30 && i<40){
//                    den2_data.child("MUC SANG DEN 2").setValue(30);
//                    dosang=30;
//                    phantramden2.setText(dosang+"%");
//                }else if(i>=40 && i<50){
//                    den2_data.child("MUC SANG DEN 2").setValue(40);
//                    dosang=40;
//                    phantramden2.setText(dosang+"%");
//                }else if(i>=50 && i<60){
//                    den2_data.child("MUC SANG DEN 2").setValue(50);
//                    dosang=50;
//                    phantramden2.setText(dosang+"%");
//                }else if(i>=60 && i<70){
//                    den2_data.child("MUC SANG DEN 2").setValue(60);
//                    dosang=60;
//                    phantramden2.setText(dosang+"%");
//                }else if(i>=70 && i<80){
//                    den2_data.child("MUC SANG DEN 2").setValue(70);
//                    dosang=70;
//                    phantramden2.setText(dosang+"%");
//                }else if(i>=80 && i<90){
//                    den2_data.child("MUC SANG DEN 2").setValue(80);
//                    dosang=80;
//                    phantramden2.setText(dosang+"%");
//                }else if(i>=90 && i<100){
//                    den2_data.child("MUC SANG DEN 2").setValue(90);
//                    dosang=90;
//                    phantramden2.setText(dosang+"%");
//                }else {
//                    den2_data.child("MUC SANG DEN 2").setValue(100);
//                    dosang=100;
//                    phantramden2.setText(dosang+"%");
//                }////////////////
            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }
            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
            }
        });

        ////DHT11
        nhietdodata = FirebaseDatabase.getInstance().getReference();
        final ValueEventListener nd_dht11 = nhietdodata.child("DHT11").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                nhietdo.setText(snapshot.getValue().toString() + " độ C");
                if (snapshot.exists()) {
                    Integer DHT11_Value = snapshot.getValue(Integer.class);
                    if (DHT11_Value > 50) sendFireNotification(1, "Phát hiện nhiệt độ thay đổi bất thường. Vui lòng kiểm tra ngay!!!");
                }
            }
            @Override
            public void onCancelled(@NonNull DatabaseError error) {
            }
        });
        //GAS
        gasdata = FirebaseDatabase.getInstance().getReference();
        final ValueEventListener gas_value = gasdata.child("GAS").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                mq2TextView.setText(snapshot.getValue().toString() + "Ppm");
                if (snapshot.exists()) {
                    Integer CO2_Value = snapshot.getValue(Integer.class);
                    if (CO2_Value > 1500) sendFireNotification(1, "Phát hiện khí độc bị rò rỉ. Vui lòng kiểm tra ngay!!!");
                }
            }
            @Override
            public void onCancelled(@NonNull DatabaseError error) {
                Toast.makeText(MainActivity2.this, "Lỗi đọc dữ liệu: " + error.getMessage(), Toast.LENGTH_SHORT).show();
            }
        });
        //HUMIDITY
        humiditydata = FirebaseDatabase.getInstance().getReference();
        final ValueEventListener humidity_value = humiditydata.child("HUMIDITY").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                humidityTextView.setText(snapshot.getValue().toString() + "%");
            }
            @Override
            public void onCancelled(@NonNull DatabaseError error) {
                Toast.makeText(MainActivity2.this, "Lỗi đọc dữ liệu: " + error.getMessage(), Toast.LENGTH_SHORT).show();
            }
        });
        //WATERSENSOR
        watersensordata = FirebaseDatabase.getInstance().getReference();
        final ValueEventListener watersensor_value = watersensordata.child("WS").addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                WaterTextView.setText(snapshot.getValue().toString() + "cm");
                Integer watersensor_Value = snapshot.getValue(Integer.class);
                if (snapshot.exists()) {
                    if(watersensor_Value < 1){
                        sendFireNotification(2,"Phát hiện bồn chứa sắp hết nước.Vui lòng kiểm tra ngay!!!");
                    }else if (watersensor_Value > 4){
                        sendFireNotification(2,"Nước sắp đầy.Vui lòng kiểm tra ngay!!!");
                    }
                }
            }
            @Override
            public void onCancelled(@NonNull DatabaseError error) {
                Toast.makeText(MainActivity2.this, "Lỗi đọc dữ liệu: " + error.getMessage(), Toast.LENGTH_SHORT).show();
            }
        });
        // xu ly nut đi ngủ
        dingu.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                isOn[2] = !isOn[2];
                databaseRef_dingu.setValue(isOn[2]);
                if(isOn[2]){
                    sendFireNotification(2,"Chế độ 'BAN ĐÊM' đã được kích hoạt!");
                }else{
                    sendFireNotification(2,"Chế độ 'BAN ĐÊM' đã được ngắt kích hoạt!");
                }
            }
        });
        // xử lyus tiết kiệm năng lượng
        tietkiemnangluong.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                isOn[3] = !isOn[3];
                databaseRef_tietkiemnangluong.setValue(isOn[3]);
                if(isOn[3]){
                    sendFireNotification(2,"Chế độ 'TIẾT KIỆM NĂNG LƯỢNG' đã được kích hoạt!");
                }else{
                    sendFireNotification(2,"Chế độ 'TIẾT KIỆM NĂNG LƯỢNG' đã được ngắt kích hoạt!");
                }
            }
        });
        // xử lý nút nhấn ban ngày
        thucday.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                isOn[4] = !isOn[4];
                databaseRef_thucday.setValue(isOn[4]);
                if(isOn[4]){
                    sendFireNotification(2,"Chế độ 'BAN NGÀY' đã được kích hoạt!");
                }else{
                    sendFireNotification(2,"Chế độ 'BAN NGÀY' đã được ngắt kích hoạt!");
                }
            }
        });
        // xử lý nút nhấn ra ngoài
        rangoai.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                isOn[5] = !isOn[5];
                databaseRef_rangoai.setValue(isOn[5]);
                if(isOn[5]){
                    sendFireNotification(2,"Chế độ 'RA NGOÀI' đã được kích hoạt!");
                }else{
                    sendFireNotification(2,"Chế độ 'RA NGOÀI' đã được ngắt kích hoạt!");
                }
            }
        });
        // xu ly nut logout
        btnlogout.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                finish();
            }
        });
    }
    //THONGBAO
    private void createNotificationChannel() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            String channelId = "channel_id";
            String channelName = "Cảnh báo cháy";
            String channelDescription = "Thông báo khi phát hiện lửa";
            int importance = NotificationManager.IMPORTANCE_HIGH;

            NotificationChannel channel = new NotificationChannel(channelId, channelName, importance);
            channel.setDescription(channelDescription);

            NotificationManager notificationManager = getSystemService(NotificationManager.class);
            if (notificationManager != null) {
                notificationManager.createNotificationChannel(channel);
            }
        }
    }
    private void sendFireNotification(int i,String issue) {
        String channelId = "channel_id"; // ID của kênh thông báo
        String title = new String();
        String message = issue;
        if(i==1){
            title = "CẢNH BÁO!";
        }else if(i==2){
            title = "LƯU Ý!";
        }
        createNotificationChannel();
        NotificationCompat.Builder builder = new NotificationCompat.Builder(this, channelId)
                .setSmallIcon(R.drawable.baseline_warning_on_24)
                .setContentTitle(title)
                .setContentText(message)
                .setPriority(NotificationCompat.PRIORITY_HIGH)
                .setAutoCancel(true);

        NotificationManager notificationManager = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);
        if (notificationManager != null) {
            notificationManager.notify(1, builder.build());
        }
    }
}