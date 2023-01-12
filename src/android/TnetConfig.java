package com.tencent.qqlive.modules.vb.tquic.impl;


public class TnetConfig {

    // Whether employ a sync-mechanism to prevent the QuicChromiumPacketReader
    // from reading too fast. If this mode is on, the data consuming speed
    // will be well controlled by the speed you process OnDataRecv().
    // So you can lower the data receiving speed by blocking this method.
    // But becareful if you do so, the next TnetQuicRequest you start will be
    // effected, because there is only one global event thread.
    // The best practice for now is to unblock all the old TnetRequestDelegates
    // before you start a new one. If there is a much stronger demand for massive
    // concurrent requests, we will improve this in future.
    public boolean mSyncRead;
    // The congestion control algorithm, it's cubic_bytes defaultly.
    public int mCongestionType;
    // Enable Congetion Optimization, it's disabled defaultly.
    public boolean mEnableCongetionOptimization;
    // Represent total timeout set by upper layer.
    public int mTotalTimeoutMillis;
    // Represent max connect time, if triggerd, OnConnectionClose would be called.
    public int mConnectTimeoutMillis;
    // Represent max packet idle time, if two packet interval time exceed it,
    // OnConnectionClose would be called.
    // 4000 milliseconds defaultly before handshake complete.
    // 600 seconds defaultly after handshake.
    public int mIdleTimeoutMillis;
    // Specify quic version, only support quic 39-46, it is 43 defaultly.
    public int mQuicVersion;

    // default is false.
    public boolean mUseRedirect;

    // default is false.
    public boolean mForceZeroRTT;

    // default is true.
    public boolean mUseSessionReuse;

    private TnetConfig(Builder builder) {
        this.mSyncRead = builder.mSyncRead;
        this.mCongestionType = builder.mCongestionType;
        this.mEnableCongetionOptimization = builder.mEnableCongetionOptimization;
        this.mTotalTimeoutMillis = builder.mTotalTimeoutMillis;
        this.mConnectTimeoutMillis = builder.mConnectTimeoutMillis;
        this.mIdleTimeoutMillis = builder.mIdleTimeoutMillis;
        this.mQuicVersion = builder.mQuicVersion;
        this.mUseRedirect = builder.mUseRedirect;
        this.mForceZeroRTT = builder.mForceZeroRTT;
        this.mUseSessionReuse = builder.mUseSessionReuse;
    }

    public boolean isSyncRead() {
        return mSyncRead;
    }

    public int getCongestionType() {
        return mCongestionType;
    }

    public boolean isCongetionOptimizationEnabled() {
        return mEnableCongetionOptimization;
    }

    public int getTotalTimeoutMillis() {
        return mTotalTimeoutMillis;
    }

    public int getConnectTimeoutMillis() {
        return mConnectTimeoutMillis;
    }

    public int getIdleTimeoutMillis() {
        return mIdleTimeoutMillis;
    }

    public int getQuicVersion() {
        return mQuicVersion;
    }

    public boolean isUseRedirect() {
        return mUseRedirect;
    }

    public boolean isForceZeroRTT() {
        return mForceZeroRTT;
    }

    public boolean mUseSessionReuse() {
        return mUseSessionReuse;
    }

    public static class Builder {

        private boolean mSyncRead;
        // The congestion control algorithm, it's cubic_bytes defaultly.
        private int mCongestionType;
        // Enable Congetion Optimization, it's disabled defaultly.
        private boolean mEnableCongetionOptimization;
        // Represent total timeout set by upper layer.
        private int mTotalTimeoutMillis;
        // Represent max connect time, if triggerd, OnConnectionClose would be called.
        private int mConnectTimeoutMillis;
        // Represent max packet idle time, if two packet interval time exceed it,
        // OnConnectionClose would be called.
        // 4000 milliseconds defaultly before handshake complete.
        // 600 seconds defaultly after handshake.
        private int mIdleTimeoutMillis;
        // Specify quic version, only support quic 39-46, it is 43 defaultly.
        private int mQuicVersion;
        // default is false.
        private boolean mUseRedirect;

        //default is false.
        private boolean mForceZeroRTT;

        // default is true.
        private boolean mUseSessionReuse;

        public Builder() {
            this.mSyncRead = false;
            this.mCongestionType = 2;
            this.mEnableCongetionOptimization = false;
            this.mTotalTimeoutMillis = 0;
            this.mConnectTimeoutMillis = 0;
            this.mIdleTimeoutMillis = 0;
            this.mQuicVersion = 43;
            this.mUseRedirect = false;
            this.mForceZeroRTT = false;
            this.mUseSessionReuse = true;
        }

        public Builder(TnetConfig tnetConfig) {
            this.mSyncRead = tnetConfig.mSyncRead;
            this.mCongestionType = tnetConfig.mCongestionType;
            this.mEnableCongetionOptimization = tnetConfig.mEnableCongetionOptimization;
            this.mTotalTimeoutMillis = tnetConfig.mTotalTimeoutMillis;
            this.mConnectTimeoutMillis = tnetConfig.mConnectTimeoutMillis;
            this.mIdleTimeoutMillis = tnetConfig.mIdleTimeoutMillis;
            this.mQuicVersion = tnetConfig.mQuicVersion;
            this.mUseRedirect = tnetConfig.mUseRedirect;
            this.mForceZeroRTT = tnetConfig.mForceZeroRTT;
            this.mUseSessionReuse = tnetConfig.mUseSessionReuse;
        }

        public Builder setSyncRead(boolean syncRead) {
            this.mSyncRead = syncRead;
            return this;
        }

        public Builder setCongestionType(int congestionType) {
            this.mCongestionType = congestionType;
            return this;
        }

        public Builder enableCongetionOptimization(boolean enableCongetionOptimization) {
            this.mEnableCongetionOptimization = enableCongetionOptimization;
            return this;
        }

        public Builder setTotalTimeoutMillis(int totalTimeoutMillis) {
            this.mTotalTimeoutMillis = totalTimeoutMillis;
            return this;
        }

        public Builder setConnectTimeoutMillis(int connectTimeoutMillis) {
            this.mConnectTimeoutMillis = connectTimeoutMillis;
            return this;
        }

        public Builder setIdleTimeoutMillis(int idleTimeoutMillis) {
            this.mIdleTimeoutMillis = idleTimeoutMillis;
            return this;
        }

        public Builder setQuicVersion(int QUICVersion) {
            this.mQuicVersion = QUICVersion;
            return this;
        }

        public Builder setUseRedirect(boolean useRedirect) {
            this.mUseRedirect = useRedirect;
            return this;
        }

        public Builder setForceZeroRTT(boolean forceZeroRTT) {
            this.mForceZeroRTT = forceZeroRTT;
            return this;
        }

        public Builder setUseSessionReuse(boolean useSessionReuse) {
            this.mUseSessionReuse = useSessionReuse;
            return this;
        }

        public TnetConfig build() {
            return new TnetConfig(this);
        }
    }
}