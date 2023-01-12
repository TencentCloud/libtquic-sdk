package com.tencent.qqlive.modules.vb.tquic.impl;

public class TnetStats {
    public boolean mIsValid;
    public boolean mIsQUIC;
    public boolean mIs0rtt;
    public boolean mIsConnectReuse;
    public long mConnectMillis;
    public long mTTfbMillis;

    // Only valid if it is quic.
    public long mCompleteMillis;
    public long mSrttMillis;
    public long mPacketsSent;
    public long mPacketsRetransmitted;

    public long mBytesSend;
    public long mBytesRetransmitted;
    public long mPacketsLost;  // Number of packets be lost when sent data.
    public long mPacketsReceived;  // Total packets received
    public long mBytesReceived; // Total bytes received including packet format.
    public long mStreamBytesReceived; // Total bytes received including duplicated data.
}