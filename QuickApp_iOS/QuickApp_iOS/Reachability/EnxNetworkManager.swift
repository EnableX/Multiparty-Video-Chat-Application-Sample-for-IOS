//
//  EnxNetworkManager.swift
//  QuickApp_iOS
//
//  Created by Daljeet Singh on 04/12/18.
//  Copyright © 2018 Daljeet Singh. All rights reserved.
//

import Foundation

import UIKit
import Foundation
import Reachability

enum ReachabilityManagerType {
    case Wifi
    case Cellular
    case None
    
}
class EnxNetworkManager: NSObject {
    var reachability: Reachability!
    
    // Create a singleton instance
    static let sharedInstance: EnxNetworkManager = { return EnxNetworkManager() }()
    override init() {
        super.init()
        // Initialise reachability
         do {
               try reachability = Reachability()
               
               } catch {
          
           }
        // Register an observer for the network status
        NotificationCenter.default.addObserver(
            self,
            selector: #selector(networkStatusChanged(_:)),
            name: .reachabilityChanged,
            object: reachability
        )
        do {
            // Start the network status notifier
            try reachability.startNotifier()
        } catch {
            print("Unable to start notifier")
        }
    }
    @objc func networkStatusChanged(_ notification: Notification) {
        // Do something globally here!
    }
    
    static func stopNotifier() -> Void {
        do {
            // Stop the network status notifier
            try (EnxNetworkManager.sharedInstance.reachability).startNotifier()
        } catch {
            print("Error stopping notifier")
        }
    }
    
    
    
    // Network is reachable
    static func isReachable() -> Bool{
        if(EnxNetworkManager.sharedInstance.reachability).connection != .none{
            return true
        }
        else{
            return false
        }
    }
    // Network is unreachable
    static func isUnreachable() -> Bool {
        if(EnxNetworkManager.sharedInstance.reachability).connection == .none{
            return true
        }
        else{
            return false
        }
    }
    
    /*
     // Network is reachable
     static func isReachable(completed: @escaping (EnxNetworkManager) -> Void) {
     if (EnxNetworkManager.sharedInstance.reachability).connection != .none {
     completed(EnxNetworkManager.sharedInstance)
     }
     }
     static func isRich() -> Bool{
     if(EnxNetworkManager.sharedInstance.reachability).connection != .none{
     return true
     }
     else{
     return false
     }
     }
     // Network is unreachable
     static func isUnreachable(completed: @escaping (EnxNetworkManager) -> Void) {
     if (EnxNetworkManager.sharedInstance.reachability).connection == .none {
     completed(EnxNetworkManager.sharedInstance)
     }
     }*/
    
    // Network is reachable via WWAN/Cellular
    static func isReachableViaWWAN(completed: @escaping (EnxNetworkManager) -> Void) {
        if (EnxNetworkManager.sharedInstance.reachability).connection == .cellular {
            completed(EnxNetworkManager.sharedInstance)
        }
    }
    
    // Network is reachable via WiFi
    static func isReachableViaWiFi(completed: @escaping (EnxNetworkManager) -> Void) {
        if (EnxNetworkManager.sharedInstance.reachability).connection == .wifi {
            completed(EnxNetworkManager.sharedInstance)
        }
    }
}
