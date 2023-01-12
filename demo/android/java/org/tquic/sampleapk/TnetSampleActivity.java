// Copyright (c) 2018 The Tencent Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
package org.tquic.sampleapk;

import org.chromium.base.ContextUtils;

import android.app.Activity;
import android.content.DialogInterface;
import android.os.Bundle;
import androidx.appcompat.app.AlertDialog;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;

import java.lang.Thread;
import java.nio.charset.Charset;

/**
 * Activity for managing the Cronet Sample.
 */
public class TnetSampleActivity extends Activity {
    private static final String TAG = TnetSampleActivity.class.getSimpleName();

    private TnetQuicRequest mTnetQuicRequest;
    private TnetQuicRequest mTnetTcpRequest;

    private String mUrl;
    private String mIP;
    private int mRequestNum;
    private String m_body = "";
    private TextView mResultText;
    private TextView mReceiveDataText;
    private int quic_receive_size;
    private String post_data_;

    private boolean dialog_open_;
    private boolean quic_reused_;
    private boolean tcp_reused_;

    private String IMSI;

    private static AlertDialog.Builder alert_build;

    class SimpleCallback extends TnetQuicRequest.Callback {
        SimpleCallback(int type) {
            job_type = type;
            receive_size = 0;
        }
        @Override
        public void onConnect(int error_code) {
            Log.i(TAG, "****** onConnected with error_code: " + error_code);
            if (error_code != 0) {
                Log.i(TAG, "****** connect failed");
                return;
            }
            if (job_type == 0) {
                mTnetQuicRequest.sendRequest(new byte[] {}, 0, true);
            } else {
                mTnetTcpRequest.sendRequest(new byte[] {}, 0, true);
            }
        }
        
        @Override
        public void onNetworkLinked() {

        }

        @Override
        public void onHeaderRecv(String header) {
            Log.i(TAG, "****** Header Completed ******");
            TnetSampleActivity.this.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    mResultText.setText(header);
                }
            });
        }
        @Override
        public void onDataRecv(byte[] body) {
            if(body == null || body.length <= 0) {
                return;
            }
            String sbody = new String(body);
            if (sbody.contains("status:")) {
                onHeaderRecv(sbody);
                return;
            }
            m_body = sbody;
        }

        @Override
        public void onComplete(int stream_error) {
          quic_receive_size = receive_size;
          String last_num_label = "Last " + (mRequestNum - 1) + " times requests. ";
          if(job_type == 0) {
              post_data_ = "";
              startWithTCP();
              PostData("quic_reuse=" + quic_reused_ + "&" + m_body, false);
              TnetSampleActivity.this.SetContent(last_num_label + "(1/2)Quic request complete");
          } else {
              PostData("&http_reuse=" + tcp_reused_ + "&" + m_body
                       + "&IMSI=" + IMSI, true);
              TnetSampleActivity.this.SetContent(last_num_label + "(2/2)Quic and TCP request complete");
              try {
                  Thread.sleep(2000);
              } catch (Exception e) {
                  e.printStackTrace();
              }
              mRequestNum = mRequestNum -1;
              Log.i(TAG, "Last " + mRequestNum + " times requests.");
              if (mRequestNum > 0) {
                  startWithURL(mUrl, mIP);
                  return;
              }
              TnetSampleActivity.this.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    promptForURL("https://");
                }
              });
              
          }
        }

        @Override
        public void onClose(int error_code, String error_str) {
            Log.i(TAG, "****** onConnection close, error is: " + error_code);
            String post_error_data;
            String last_num_label = "Last " + (mRequestNum - 1) + " times requests. ";
            if (job_type == 0) {
                post_error_data = "error_quic=";
            } else {
                post_error_data = "&error_http=";
            }
            post_error_data = post_error_data + error_str;

            if (dialog_open_) {return;}
            PostData(post_error_data, true);
            post_data_ = "";
            TnetSampleActivity.this.SetContent(last_num_label + "Request Error: " + error_str);
            if (job_type == 2) {
                mRequestNum = mRequestNum -1;
                Log.i(TAG, "Last " + mRequestNum + " times requests.");
                if (mRequestNum > 0) {
                    startWithURL(mUrl, mIP);
                    return;
                }
            }
            TnetSampleActivity.this.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    promptForURL("https://");
                }
            });
        }

        private int job_type;
        private int receive_size;
        private TnetQuicRequest request_;
    }


    private void PostData(String stat_data, boolean fin) {
        post_data_ = post_data_ + stat_data;
        if (!fin) {
            return;
        }
        String uri = "http://tgwsc.qq.com/tquic_app_data.html?";
        try {
            URL url = new URL(uri + post_data_);
            HttpURLConnection connection = (HttpURLConnection) url.openConnection();

            connection.setDoOutput(true);
            connection.setRequestMethod("GET");
            connection.setRequestProperty("Content-Type",
                "application/x-www-form-urlencoded; charset=UTF-8");
            connection.addRequestProperty("Connection","Close");
            connection.connect();
            InputStream inputStream=connection.getInputStream();
            byte[] data=new byte[1024];
            StringBuffer sb=new StringBuffer();
            int length=0;
            while ((length=inputStream.read(data))!=-1){
                String s=new String(data, Charset.forName("utf-8"));
                sb.append(s);
            }
            inputStream.close();
            connection.disconnect();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    public void SetContent(String contents) {
        TnetSampleActivity.this.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mReceiveDataText.setText(contents);
            }
        });
    }

    @Override
    protected void onCreate(final Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        ContextUtils.initApplicationContext(this);

        String SimName = NetUtils.getOperatorName(this);
        int NetType = NetUtils.getNetworkState(this);

        IMSI = SimName + "&nettype=" + NetType;
        Log.i(TAG, "Get IMSI: " + IMSI);
        dialog_open_ = false;
        quic_reused_ = false;
        tcp_reused_ = false;

        System.loadLibrary("tquic_jni");
        // System.loadLibrary("tnetquic");
        setContentView(R.layout.activity_main);
        mResultText = (TextView) findViewById(R.id.resultView);
        mReceiveDataText = (TextView) findViewById(R.id.dataView);
        
        quic_receive_size = 0;
        
        TnetConfig jTnetConfig = new TnetConfig.Builder()
                .setCongestionType(2)
                .setConnectTimeoutMillis(66*1000)
                .setIdleTimeoutMillis(80*1000)
                .setTotalTimeoutMillis(90*1000)
                .setQuicVersion(43)
                .build();

        SimpleCallback callback_quic = new SimpleCallback(0);
        mTnetQuicRequest = new TnetQuicRequest(callback_quic, jTnetConfig, 0);
        SimpleCallback callback_tcp = new SimpleCallback(2);
        mTnetTcpRequest = new TnetQuicRequest(callback_tcp, jTnetConfig, 2);

        String appUrl = (getIntent() != null ? getIntent().getDataString() : null);
        if (appUrl == null) {
            promptForURL("https://");
        } else {
            startWithURL(appUrl);
        }
    }

    private void promptForURL(String url) {
        Log.i(TAG, "No URL provided via intent, prompting user...");
        //AlertDialog.Builder alert = new AlertDialog.Builder(this);
        //alert.setTitle("Enter a URL");
        if (dialog_open_ == true) {
            Log.i(TAG, "Dialog has already been opened...");
            return;
        }
        dialog_open_ = true;
        if (alert_build == null) {
            alert_build = new AlertDialog.Builder(this);
            alert_build.setTitle("Enter a URL");
        }
        LayoutInflater inflater = getLayoutInflater();
        View alertView = inflater.inflate(R.layout.dialog_url, null);
        final EditText urlInput = (EditText) alertView.findViewById(R.id.urlText);
        urlInput.setText(url);
        final EditText ipInput = (EditText) alertView.findViewById(R.id.postText);
        alert_build.setView(alertView);

        final EditText reuseInput = (EditText) alertView.findViewById(R.id.reuseText);
        
        alert_build.setPositiveButton("Load", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int button) {
                String url = urlInput.getText().toString();
                String ip_address = ipInput.getText().toString();
                
                String is_reuse = reuseInput.getText().toString();
                try {
                    mRequestNum = Integer.parseInt(is_reuse);
                } catch(Exception e) {
                    mRequestNum = 0;
                }

                Log.i(TAG, "Consecutive request nums is " + mRequestNum);
                dialog_open_ = false;
                startWithURL(url, ip_address);
            }
        });
        alert_build.show();
    }

    private void startWithURL(String url) {
        startWithURL(url, null);
    }

    private void startWithURL(String url, String ip_address) {
        Log.i(TAG, "Tnet start");
        quic_reused_ = false;
        tcp_reused_ = false;
        mUrl = url;
        mIP = ip_address;
        if (mRequestNum % 2 != 0) {
            mTnetQuicRequest.CancelRequest();    
        }
        if (mRequestNum != 0 && mTnetQuicRequest.isConnectCompleted()) {
          quic_reused_ = true;
          mTnetQuicRequest.sendRequest(new byte[]{},0, true);
          return;
        }
        mTnetQuicRequest.connect(url, ip_address);
    }

    public void startWithTCP() {
        Log.i(TAG, "Tnet TCP start");
        if (mRequestNum != 0 && mTnetTcpRequest.isConnectCompleted()) {
          tcp_reused_ = true;
          mTnetTcpRequest.sendRequest(new byte[]{}, 0,true);
          return;
        }
        mTnetTcpRequest.connect(mUrl, mIP);
    }
}
