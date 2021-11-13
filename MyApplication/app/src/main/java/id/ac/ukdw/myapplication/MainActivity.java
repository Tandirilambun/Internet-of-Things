package id.ac.ukdw.myapplication;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;

import android.widget.CompoundButton;
import android.widget.TextView;
import android.widget.ToggleButton;


import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class MainActivity extends AppCompatActivity {

    ToggleButton buttonPower;

    TextView status;
    TextView kelembaban;

    String valueStatus;
    String valueLembab;

    DatabaseReference dref;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        buttonPower = (ToggleButton) findViewById(R.id.buttonPower);

        status = (TextView) findViewById(R.id.txtView_valueStatus);
        kelembaban = (TextView) findViewById(R.id.txtView_valueLembab);

        dref = FirebaseDatabase.getInstance().getReference();
        dref.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                valueLembab = snapshot.child("Path/Kelembaban").getValue().toString();
                kelembaban.setText(valueLembab);

                valueStatus = snapshot.child("Path/Status").getValue().toString();
                status.setText(valueStatus);
            }

            @Override
            public void onCancelled(@NonNull DatabaseError error) {

            }
        });
        buttonPower.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked){
                    DatabaseReference dhtRef = FirebaseDatabase.getInstance().getReference("Path/dht");
                    dhtRef.setValue(1);
                }
                else
                {
                    DatabaseReference dhtRef = FirebaseDatabase.getInstance().getReference("Path/dht");
                    dhtRef.setValue(0);
                }
            }
        });
    }
}