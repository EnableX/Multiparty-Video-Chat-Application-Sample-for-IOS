/**
 @enum EnxClientState
 */
typedef NS_ENUM(NSInteger, EnxClientState) {
    /// Disconnected
    EnxClientStateDisconnected,
    /// Ready to signaling
    EnxClientStateReady,
    /// Signaling proccess
    EnxClientStateConnecting,
    /// Signlaning done
    EnxClientStateConnected,
};
