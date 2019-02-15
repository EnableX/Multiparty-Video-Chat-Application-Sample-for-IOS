//
//  EnxServicesClass.swift
//  QuickApp_iOS
//
//  Created by Daljeet Singh on 04/12/18.
//  Copyright © 2018 Daljeet Singh. All rights reserved.
//

import Foundation
import UIKit

class EnxServicesClass: NSObject {
    
    // MARK: - Create Room
    /**
     Input Parameter : - App Id and App Key
     Return :- EnxRoomInfoModel
     **/
    class func createRoom(completion:@escaping (EnxRoomInfoModel) -> ()){
        //create the url with URL
        let url = URL(string: kBasedURL + "createRoom")!
        //Create A session Object
        let session = URLSession.shared
        //Now create the URLRequest object using the url object
        var request = URLRequest(url: url)
        request.httpMethod = "POST"
        request.addValue("application/json", forHTTPHeaderField: "Content-Type")
        request.addValue("application/json", forHTTPHeaderField: "Accept")
        //Create Base64 Encription
        request.addValue("Basic \(createBase64encoding())", forHTTPHeaderField: "Authorization")
        //create dataTask using the session object to send data to the server
        let tast = session.dataTask(with: request as URLRequest){(data,response, error) in
            guard error == nil else{
                let roomdataModel = EnxRoomInfoModel()
                roomdataModel.error = error?.localizedDescription
                completion(roomdataModel)
                return}
            guard let data = data else {
                let roomdataModel = EnxRoomInfoModel()
                roomdataModel.isRoomFlag = false
                completion(roomdataModel)
                return}
            do{
                if let responseValue = try JSONSerialization.jsonObject(with: data, options: .mutableContainers) as? [String : Any]{
                    print("Create Room response",responseValue)
                    let roomdataModel = EnxRoomInfoModel()
                    if (responseValue["result"] as! Int) == 0{
                        if let respValue = responseValue["room"] as? [String : Any]{
                            roomdataModel.room_id = (respValue["room_id"] as! String)
                            let settingValue = respValue["settings"] as! [String : Any]
                            roomdataModel.mode = (settingValue["mode"] as! String)
                            roomdataModel.isRoomFlag = true
                        }
                        else{
                            roomdataModel.isRoomFlag = false
                        }
                    }
                    else{
                        roomdataModel.isRoomFlag = false
                    }
                    completion(roomdataModel)
                }
            }catch{
                let roomdataModel = EnxRoomInfoModel()
                roomdataModel.error = error.localizedDescription
                completion(roomdataModel)
                print(error.localizedDescription)
            }
        }
        tast.resume()
    }
    
    class func createBase64encoding() ->String{
        let headerString = String(format: "%@:%@",userName ,password)
        let date = headerString.data(using: .utf8)
        return date!.base64EncodedString(options: NSData.Base64EncodingOptions(rawValue: 0))
    }
    
    // MARK: - Join Room With Room ID
    /**
     Input Parameter : - RoomId
     Return :- EnxRoomInfoModel
     **/
    class func fetchRoomInfoWithRoomId(roomId : String , completion:@escaping (EnxRoomInfoModel) -> ()){
        let url = URL(string: kBasedURL + "getRoom/\(roomId)")!
        //Create A session Object
        let session = URLSession.shared
        //Now create the URLRequest object using the url object
        var request = URLRequest(url: url)
        request.httpMethod = "GET"
        request.addValue("application/json", forHTTPHeaderField: "Content-Type")
        request.addValue("application/json", forHTTPHeaderField: "Accept")
        //create dataTask using the session object to send data to the server
        let tast = session.dataTask(with: request as URLRequest){(data,response, error) in
            guard error == nil else{
                let roomdataModel = EnxRoomInfoModel()
                roomdataModel.error = error?.localizedDescription
                completion(roomdataModel)
                return}
            guard let data = data else {
                let roomdataModel = EnxRoomInfoModel()
                roomdataModel.isRoomFlag = false
                completion(roomdataModel)
                return}
            do{
                if let responseValue = try JSONSerialization.jsonObject(with: data, options: JSONSerialization.ReadingOptions.mutableContainers) as? [String:Any]{
                    print("response Value",responseValue)
                    let roomdataModel = EnxRoomInfoModel()
                    if (responseValue["result"] as! Int) == 0{
                        if let respValue = responseValue["room"] as? [String : Any]{
                            roomdataModel.room_id = (respValue["room_id"] as! String)
                            let settingValue = respValue["settings"] as! [String : Any]
                            roomdataModel.mode = (settingValue["mode"] as! String)
                            roomdataModel.isRoomFlag = true
                        }
                        else{
                            roomdataModel.isRoomFlag = false
                            roomdataModel.error = nil
                        }
                    }
                    else{
                        roomdataModel.isRoomFlag = false
                        roomdataModel.error = (responseValue["error"] as! String)
                    }
                    completion(roomdataModel)
                }
                
            }catch {
                let roomdataModel = EnxRoomInfoModel()
                roomdataModel.error = error.localizedDescription
                completion(roomdataModel)
            }
        }
        tast.resume()
    }
    /*
     // MARK: - Join Room With Pin
     /**
     Input Parameter : - Pin Nummer
     Return :- EnxRoomInfoModel
     **/
     class func fetchRoomInfoWithPin(parameter : String , completion:@escaping (EnxRoomInfoModel) -> ()){
     let param = ["pin" : parameter]
     //create the url with URL
     let url = URL(string: kBasedURL + "getRoomByPin")!
     //Create A session Object
     let session = URLSession.shared
     //Now create the URLRequest object using the url object
     var request = URLRequest(url: url)
     request.httpMethod = "POST"
     do{
     request.httpBody = try JSONSerialization.data(withJSONObject: param, options:.prettyPrinted)
     } catch let error {
     print(error.localizedDescription)
     }
     request.addValue("application/json", forHTTPHeaderField: "Content-Type")
     request.addValue("application/json", forHTTPHeaderField: "Accept")
     //create dataTask using the session object to send data to the server
     let tast = session.dataTask(with: request as URLRequest){(data,response, error) in
     guard error == nil else{
     let roomdataModel = EnxRoomInfoModel()
     roomdataModel.error = error
     completion(roomdataModel)
     return}
     guard let data = data else {
     let roomdataModel = EnxRoomInfoModel()
     roomdataModel.isRoomFlag = false
     completion(roomdataModel)
     return}
     do{
     if let responseValue = try JSONSerialization.jsonObject(with: data, options: .mutableContainers) as? [String : String]{
     let roomdataModel = EnxRoomInfoModel()
     roomdataModel.role = responseValue["role"]
     roomdataModel.room_id = responseValue["room_id"]
     roomdataModel.mode = responseValue["mode"]
     roomdataModel.isRoomFlag = true
     completion(roomdataModel)
     }
     }catch{
     let roomdataModel = EnxRoomInfoModel()
     roomdataModel.error = error
     completion(roomdataModel)
     print(error.localizedDescription)
     }
     }
     tast.resume()
     }*/
    // MARK: - featchToken
    /**
     Input Parameter : - [String : String]
     Return :- EnxRoomInfoModel
     **/
    class func featchToken(requestParam : [String: String] , completion:@escaping (EnxTokenModel) -> ()){
        //create the url with URL
        let url = URL(string: kBasedURL + "createToken")!
        //Create A session Object
        let session = URLSession.shared
        //Now create the URLRequest object using the url object
        var request = URLRequest(url: url)
        request.httpMethod = "POST"
        do{
            request.httpBody = try JSONSerialization.data(withJSONObject: requestParam, options:.prettyPrinted)
        } catch let error {
            print(error.localizedDescription)
        }
        request.addValue("application/json", forHTTPHeaderField: "Content-Type")
        request.addValue("application/json", forHTTPHeaderField: "Accept")
        //create dataTask using the session object to send data to the server
        let tast = session.dataTask(with: request as URLRequest){(data,response, error) in
            guard error == nil else{
                let tokenModel = EnxTokenModel()
                tokenModel.error = error?.localizedDescription
                completion(tokenModel)
                return}
            guard let data = data else {
                let tokenModel = EnxTokenModel()
                tokenModel.token = ""
                completion(tokenModel)
                return}
            do{
                if let responseValue = try JSONSerialization.jsonObject(with: data, options: .mutableContainers) as? [String : Any]{
                    print(responseValue)
                    let tokenModel = EnxTokenModel()
                    if let token = responseValue["token"] as? String{
                        tokenModel.token = token
                    }
                    else{
                        tokenModel.token = nil
                    }
                    
                    completion(tokenModel)
                }
            }catch{
                let tokenModel = EnxTokenModel()
                tokenModel.error = error.localizedDescription
                completion(tokenModel)
                print(error.localizedDescription)
            }
        }
        tast.resume()
        
    }
}
