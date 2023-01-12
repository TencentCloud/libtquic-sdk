// Copyright (c) 2018 The Tencent Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package org.tquic.sampleapk;

public class TnetQuicRequest {
    public abstract static class Callback {
        public abstract void onConnect(int error_code) throws Exception;
        public abstract void onNetworkLinked() throws Exception;
        public abstract void onHeaderRecv(String header) throws Exception;
        public abstract void onDataRecv(byte[] body) throws Exception;
        public abstract void onComplete(int stream_error) throws Exception;
        public abstract void onClose(int error_code, String error_str) throws Exception;
    }

    private long mTnetQuicAdapter;
    private final Callback mCallback; 

    TnetQuicRequest(Callback callback, TnetConfig jConfig, int type) {
        mCallback = callback;
        try {
          mTnetQuicAdapter =
                  nativeCreateTnetQuicAdapter(jConfig, type);
        } catch (RuntimeException e) {
          throw e;
        }
    }

    public void connect(String url, String ip_address) {
        nativeConnect(mTnetQuicAdapter, url, ip_address);
    }

    public void connectWithDomain(String url) {
        nativeConnectWithDomain(mTnetQuicAdapter, url);
    }

    public boolean connectAndSend(String url,
                                  byte[] body,
                                  int length) {
        nativeConnectAndSend(mTnetQuicAdapter, url, body, length);
    }

    public void addHeaders(String key, String value) {
        nativeAddHeaders(mTnetQuicAdapter, key, value);
    }

    public boolean isConnectCompleted() {
        return nativeIsConnectCompleted(mTnetQuicAdapter);
    }

    public boolean sendRequest(byte[] body, int length, boolean fin) {
        return nativeSendRequest(mTnetQuicAdapter, body, length, fin);
    }

    public boolean isRequestFinished() {
        return nativeRequestFinished(mTnetQuicAdapter);
    }

    public void CancelRequest() {
        nativeCancelRequest(mTnetQuicAdapter);
    }

    public void Destroy() {
        nativeDestroy(mTnetQuicAdapter);
    }

    public void GetTnetStates(TnetStats jstats) {
        nativeGetTnetStates(mTnetQuicAdapter, jstats);
    }

    private void onConnect(int error_code) {
        try {
            mCallback.onConnect(error_code);
        } catch (Exception e) {
            return;
        }
    }

    private void onNetworkLinked() {
        try {
            mCallback.onNetworkLinked();
        } catch (Exception e) {
            return;
        }
    }

    private void onDataRecv(byte[] body) {
        try {
            mCallback.onDataRecv(body);
        } catch (Exception e) {
            return;
        }
    }

    private void onComplete(int stream_error) {
        try {
            mCallback.onComplete(stream_error);
        } catch (Exception e) {
            return;
        }
    }

    private void onClose(int error_code, String error_str) {
        try {
            mCallback.onClose(error_code, error_str);
        } catch (Exception e) {
            return;
        }
    }

    public static void SetExpId(int expid) {
        nativeSetExpid(expid);
    }

    public static void PreConnect(String url) {
        nativeTquicPreConnect(url);
    }

    private native long nativeCreateTnetQuicAdapter(TnetConfig jConfig, int type);
    private native void nativeSetExpid(int expid);
    private native void nativeTquicPreConnect(String url);
    private native void nativeConnect(long nativePtr, String url, String ip);
    private native void nativeConnectWithDomain(long nativePtr, String url);
    private native void nativeConnectAndSend(long nativePtr,
                                             String url,
                                             byte[] body,
                                             int length);
    private native void nativeAddHeaders(long nativePtr, String key, String value);
    private native boolean nativeIsConnectCompleted(long nativePtr);
    private native boolean nativeSendRequest(long nativePtr, byte[] body, int length, boolean fin);
    private native boolean nativeRequestFinished(long nativePtr);
    private native void nativeCancelRequest(long nativePtr);
    private native void nativeDestroy(long nativePtr);
    private native void nativeGetTnetStates(long nativePtr, TnetStats jstats);

}
  
