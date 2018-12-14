//
//  EnxConfrenceViewController.swift
//  QuickApp_iOS
//
//  Created by Daljeet Singh on 04/12/18.
//  Copyright © 2018 Daljeet Singh. All rights reserved.
//

import Foundation

import UIKit
import EnxRTCiOS
import SVProgressHUD
class EnxConfrenceViewController: UIViewController {
     @IBOutlet weak var cameraBTN: UIButton!
    @IBOutlet weak var collectionView: UICollectionView!
    @IBOutlet weak var publisherNameLBL: UILabel!
    @IBOutlet weak var subscriberNameLBL: UILabel!
    @IBOutlet weak var messageLBL: UILabel!
    @IBOutlet weak var localPlayerView: EnxPlayerView!
    @IBOutlet weak var mainPlayerView: EnxPlayerView!
    @IBOutlet weak var optionsView: UIView!
    @IBOutlet weak var optionViewButtonlayout: NSLayoutConstraint!
    var roomInfo : EnxRoomInfoModel!
    var param : [String : Any] = [:]
    var remoteRoom : EnxRoom!
    var objectJoin : EnxRtc!
    var localStream : EnxStream!
    var listOfParticipantInRoom  = [Any]()
    var streamArray = [Any]()
    override func viewDidLoad() {
        super.viewDidLoad()
        UIApplication.shared.isIdleTimerDisabled = true
        localPlayerView.layer.cornerRadius = 8.0
        localPlayerView.layer.borderWidth = 2.0
        localPlayerView.layer.borderColor = UIColor.blue.cgColor
        localPlayerView.layer.masksToBounds = true
        optionsView.layer.cornerRadius = 8.0
        //optionViewButtonlayout.constant = -100
        //        let tapGuester = UITapGestureRecognizer(target: self, action: #selector(handleSingleTap))
        //        tapGuester.numberOfTapsRequired = 1
        //        self.view.addGestureRecognizer(tapGuester)
        
        // Adding Pan Gesture for localPlayerView
        let localViewGestureRecognizer = UIPanGestureRecognizer(target: self, action: #selector(didChangePosition))
        localPlayerView.addGestureRecognizer(localViewGestureRecognizer)
        
        objectJoin = EnxRtc()
        self.createToken()
        self.navigationItem.hidesBackButton = true
        // Do any additional setup after loading the view.
    }
    // MARK: - didChangePosition
    /**
     This method will change the position of localPlayerView
     Input parameter :- UIPanGestureRecognizer
     **/
    @objc func didChangePosition(sender: UIPanGestureRecognizer) {
        let location = sender.location(in: view)
        if sender.state == .began {
        } else if sender.state == .changed {
            if(location.x <= (UIScreen.main.bounds.width - (self.localPlayerView.bounds.width/2)) && location.x >= self.localPlayerView.bounds.width/2) {
                self.localPlayerView.frame.origin.x = location.x
                localPlayerView.center.x = location.x
            }
            if(location.y <= (UIScreen.main.bounds.height - (self.localPlayerView.bounds.height + 40)) && location.y >= (self.localPlayerView.bounds.height/2)+20){
                self.localPlayerView.frame.origin.y = location.y
                localPlayerView.center.y = location.y
            }
            
        } else if sender.state == .ended {
            print("Gesture ended")
        }
    }
    
    // MARK: - createTokrn
    /**
     input parameter - Nil
     Return  - Nil
     This method will initiate the Room for stream
     **/
    private func createToken(){
        guard EnxNetworkManager.isReachable() else {
            self.showAleartView(message:"Kindly check your Network Connection", andTitles: "OK")
            return
        }
        let inputParam : [String : String] = ["name" :roomInfo.participantName , "role" :  roomInfo.role ,"roomId" : roomInfo.room_id, "user_ref" : "2236"]
        SVProgressHUD.show()
        EnxServicesClass.featchToken(requestParam: inputParam, completion:{tokenModel  in
            DispatchQueue.main.async {
                //  Success Response from server
                if let token = tokenModel.token {
                    
                    let videoSize : NSDictionary =  ["minWidth" : 720 , "minHeight" : 480 , "maxWidth" : 1280, "maxHeight" :720]
                    
                    let localStreamInfo : NSDictionary = ["video" : self.param["video"]! ,"audio" : self.param["audio"]! ,"data" :self.param["chat"]! ,"name" :self.roomInfo.participantName!,"type" : "public" ,"maxVideoBW" : 400 ,"minVideoBW" : 300 , "videoSize" : videoSize]
                    
                    self.localStream = self.objectJoin.joinRoom(token, delegate: self, publishStreamInfo: (localStreamInfo as! [AnyHashable : Any]))
                    self.localStream.delegate = self as EnxStreamDelegate
                }
                    //Handel if Room is full
                else if (tokenModel.token == nil && tokenModel.error == nil){
                    self.showAleartView(message:"Token Denied. Room is full.", andTitles: "OK")
                }
                    //Handeling server error
                else{
                    print(tokenModel.error)
                    self.showAleartView(message:tokenModel.error, andTitles: "OK")
                }
                SVProgressHUD.dismiss()
            }
        })
        
    }
    // MARK: - Show Alert
    /**
     Show Alert Based in requirement.
     Input parameter :- Message and Event name for Alert
     **/
    private func showAleartView(message : String, andTitles : String){
        let alert = UIAlertController(title: " ", message: message, preferredStyle: UIAlertController.Style.alert)
        let action = UIAlertAction(title: andTitles, style: .default) { (action:UIAlertAction) in
            self.navigationController?.popViewController(animated: true)
        }
        alert.addAction(action)
        self.present(alert, animated: true, completion: nil)
    }
    /*  // MARK: - View Tap Event
     /**
     Its method will hide/unhide option View
     **/
     @objc func handleSingleTap(sender : UITapGestureRecognizer){
     if optionViewButtonlayout.constant >= 0{
     UIView.animate(withDuration: 1, delay: 0, options: .curveEaseIn, animations: {
     self.optionViewButtonlayout.constant = -100
     }, completion: nil)
     }
     else{
     UIView.animate(withDuration: 1, delay: 0, options: .curveEaseOut, animations: {
     self.optionViewButtonlayout.constant = 10
     }, completion: nil)
     }
     }*/
    // MARK: - Mute/Unmute
    /**
     Input parameter : - Button Property
     OutPut : - Nil
     Its method will Mute/Unmute sound and change Button Property.
     **/
    @IBAction func muteUnMuteEvent(_ sender: UIButton) {
        guard remoteRoom != nil else {
            return
        }
        localStream.signalingChannel = remoteRoom.signalingChannel
        if sender.isSelected {
            localStream.muteSelfAudio(false)
            sender.isSelected = false
        }
        else{
            localStream.muteSelfAudio(true)
            sender.isSelected = true
        }
    }
    // MARK: - Camera On/Off
    /**
     Input parameter : - Button Property
     OutPut : - Nil
     Its method will On/Off Camera and change Button Property.
     **/
    @IBAction func cameraOnOffEvent(_ sender: UIButton) {
        guard remoteRoom != nil else {
            return
        }
        localStream.signalingChannel = remoteRoom.signalingChannel
        if sender.isSelected {
            localStream.muteSelfVideo(false)
            sender.isSelected = false
            cameraBTN.isEnabled = true
        }
        else{
            localStream.muteSelfVideo(true)
            sender.isSelected = true
            cameraBTN.isEnabled = false
        }
    }
    // MARK: - Camera Angle
    /**
     Input parameter : - Button Property
     OutPut : - Nil
     Its method will change Camera Angle and change Button Property.
     **/
    @IBAction func changeCameraAngle(_ sender: UIButton) {
        localStream.switchCamera()
    }
    // MARK: - Speaker On/Off
    /**
     Input parameter : - Button Property
     OutPut : - Nil
     Its method will On/Off Speaker and change Button Property.
     **/
    @IBAction func speakerOnOffEvent(_ sender: UIButton) {
        guard remoteRoom != nil else {
            return
        }
        if sender.isSelected {
            remoteRoom.speakerActive(true)
            sender.isSelected = false
        }
        else{
            remoteRoom.speakerActive(false)
            sender.isSelected = true
        }
    }
    // MARK: - End Call
    /**
     Input parameter : - Any
     OutPut : - Nil
     Its method will Closed Call and exist from Room
     **/
    @IBAction func endCallEvent(_ sender: Any) {
        self.leaveRoom()
        
    }
    // MARK: - Leave Room
    /**
     Input parameter : - Nil
     OutPut : - Nil
     Its method will exist from Room
     **/
    private func leaveRoom(){
        UIApplication.shared.isIdleTimerDisabled = false
        remoteRoom?.disconnect()
        self.navigationController?.popViewController(animated: true)
    }
    
    /*
     // MARK: - Navigation
     
     // In a storyboard-based application, you will often want to do a little preparation before navigation
     override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
     // Get the new view controller using segue.destination.
     // Pass the selected object to the new view controller.
     }
     */
    
}
/*
 // MARK: - Extension
 Delegates Methods
 */
extension EnxConfrenceViewController : EnxRoomDelegate, EnxStreamDelegate {
    //Mark - EnxRoom Delegates
    /*
     This Delegate will notify to User Once he got succes full join Room
     */
    func room(_ room: EnxRoom?, didConnect roomMetadata: [AnyHashable : Any]?) {
        remoteRoom = room
        remoteRoom.publish(localStream)
        remoteRoom.setTalkerCount(4)
        if remoteRoom.isRoomActiveTalker{
            if let name = remoteRoom.whoami()!["name"] {
                publisherNameLBL.text = (name as! String)
                localPlayerView.bringSubviewToFront(publisherNameLBL)
                
            }
            localStream.attachRenderer(localPlayerView)
            localPlayerView.contentMode = UIView.ContentMode.scaleAspectFill        }else{
            localStream.attachRenderer(mainPlayerView)
            mainPlayerView.contentMode = UIView.ContentMode.scaleAspectFill
        }
        if listOfParticipantInRoom.count >= 1 {
            listOfParticipantInRoom.removeAll()
        }
        listOfParticipantInRoom.append(roomMetadata!["userList"] as! [Any])
        print(listOfParticipantInRoom);
    }
    /*
     This Delegate will notify to User Once he Getting error in joining room
     */
    func room(_ room: EnxRoom?, didError reason: String?) {
        self.showAleartView(message:reason!, andTitles: "OK")
    }
    /*
     This Delegate will notify to  User Once he Publisg Stream
     */
    func room(_ room: EnxRoom?, didPublishStream stream: EnxStream?) {
        //To Do
    }
    /*
     This Delegate will notify to  User Once he Unpublisg Stream
     */
    func room(_ room: EnxRoom?, didUnpublishStream stream: EnxStream?) {
        //To Do
    }
    /*
     This Delegate will notify to User if any new person added to room
     */
    func room(_ room: EnxRoom?, didAddedStream stream: EnxStream?) {
        room!.subscribe(stream!)
    }
    /*
     This Delegate will notify to User if any new person Romove from room
     */
    func room(_ room: EnxRoom?, didRemovedStream stream: EnxStream?) {
        //To Do
        if stream == nil{
            subscriberNameLBL.isHidden = true
        }
    }
    /*
     This Delegate will notify to User to subscribe other user stream
     */
    func room(_ room: EnxRoom?, didSubscribeStream stream: EnxStream?) {
        //To Do
    }
    /*
     This Delegate will notify to User to Unsubscribe other user stream
     */
    func room(_ room: EnxRoom?, didUnSubscribeStream stream: EnxStream?) {
        //To Do
    }
    /*
     This Delegate will notify to User if Room Got discunnected
     */
    func roomDidDisconnected(_ status: EnxRoomStatus) {
        self.leaveRoom()
    }
    /*
     This Delegate will notify to User if any person join room
     */
    func room(_ room: EnxRoom?, userDidJoined Data: [Any]?) {
        //listOfParticipantInRoom.append(Data!)
    }
    /*
     This Delegate will notify to User if any person got discunnected
     */
    func room(_ room: EnxRoom?, userDidDisconnected Data: [Any]?) {
        
    }
    /*
     This Delegate will notify to User if any person got discunnected
     */
    func room(_ room: EnxRoom?, didChange status: EnxRoomStatus) {
        //To Do
    }
    /*
     This Delegate will notify to User once any stream got publish
     */
    func room(_ room: EnxRoom?, didReceiveData data: [AnyHashable : Any]?, from stream: EnxStream?) {
        //To Do
    }
    /*
     This Delegate will notify to User to get updated attributes of particular Stream
     */
    func room(_ room: EnxRoom?, didUpdateAttributesOf stream: EnxStream?) {
        //To Do
    }
    /*
     This Delegate will notify to User if any new User Reconnect the room
     */
    func room(_ room: EnxRoom?, didReconnect reason: String?) {
        //To Do
    }
    /*
     This Delegate will notify to User with active talker list
     */
    func room(_ room: EnxRoom?, activeTalkerList Data: [Any]?) {
        //To Do
        if(streamArray.count > 0){
            streamArray.removeAll()
            collectionView.reloadData()
        }
        guard let tempDict = Data?[0] as? [String : Any], Data!.count>0 else {
            messageLBL.text = "Please wait till other participant join."
            messageLBL.isHidden = false
            subscriberNameLBL.isHidden = true
            mainPlayerView.isHidden = true
            return
        }
        let activeListArray = tempDict["activeList"] as? [Any]
        if (activeListArray?.count == 0){
            
            messageLBL.text = "Please wait till other participant join."
            messageLBL.isHidden = false
            subscriberNameLBL.isHidden = true
            mainPlayerView.isHidden = true
            
        }
        else{
            
            for (index,active) in (activeListArray?.enumerated())! {
                // Do this
                let remoteStreamDict = remoteRoom.streamsByStreamId as! [String : Any]
                let mostActiveDict = active as! [String : Any]
                let streamId = String(mostActiveDict["streamId"] as! Int)
                let stream = remoteStreamDict[streamId] as! EnxStream
                if(index == 0){
                    mainPlayerView.isHidden = false
                    subscriberNameLBL.isHidden = false
                    messageLBL.isHidden = true
                    
                    stream.streamAttributes = ["name" : mostActiveDict["name"] as! String]
                    stream.mediaType = (mostActiveDict["mediatype"] as! String)
                    stream.detachRenderer()
                    stream.attachRenderer(mainPlayerView)
                    subscriberNameLBL.text = mostActiveDict["name"] as? String
                    mainPlayerView.bringSubviewToFront(subscriberNameLBL)
                    mainPlayerView.contentMode = UIView.ContentMode.scaleAspectFill
                }
                else{
                   streamArray.append(stream)
                   collectionView.reloadData()
                }
            }
            
        }
    }
    //Mark- EnxStreamDelegate Delegate
    /*
     This Delegate will notify to current User If User will do Self Stop Video
     */
    func stream(_ stream: EnxStream?, didSelfMuteVideo data: [Any]?) {
        //To Do
    }
    /*
     This Delegate will notify to current User If User will do Self Start Video
     */
    func stream(_ stream: EnxStream?, didSelfUnmuteVideo data: [Any]?) {
        //To Do
    }
    /*
     This Delegate will notify to current User If User will do Self Mute Audio
     */
    func stream(_ stream: EnxStream?, didSelfMuteAudio data: [Any]?) {
        //To Do
    }
    /*
     This Delegate will notify to current User If User will do Self UnMute Audio
     */
    func stream(_ stream: EnxStream?, didSelfUnmuteAudio data: [Any]?) {
        //To Do
    }
    /*
     This Delegate will notify to current User If any user has stoped There Video or current user Video
     */
    func didVideoEvents(_ data: [AnyHashable : Any]?) {
        //To Do
    }
    /*
     This Delegate will notify to current User If any user has stoped There Audio or current user Video
     */
    func didAudioEvents(_ data: [AnyHashable : Any]?) {
        //To Do
    }
}

extension EnxConfrenceViewController : UICollectionViewDelegate,UICollectionViewDataSource,UICollectionViewDelegateFlowLayout {
    
   // func collectionView(_ collectionView: UICollectionView, layout collectionViewLayout: UICollectionViewLayout, sizeForItemAt indexPath: IndexPath) -> CGSize {
     //   let space : CGFloat = 30
      //  let size:CGFloat = (collectionView.frame.size.width - space) / 3.0
     //   return CGSize(width: size, height: size)
   // }
    
    func collectionView(_ collectionView: UICollectionView, numberOfItemsInSection section: Int) -> Int {
        return streamArray.count
    }
    func collectionView(_ collectionView: UICollectionView, cellForItemAt indexPath: IndexPath) -> UICollectionViewCell {
        let cell = collectionView.dequeueReusableCell(withReuseIdentifier: "customeCell", for: indexPath) as! EnxPlayerCollectionViewCell
        let stream = streamArray[indexPath.row] as! EnxStream
        stream.detachRenderer()
        cell.playerView.isHidden = true
        stream.attachRenderer(cell.playerView)
        cell.playerView.isHidden = false
        cell.playerView.contentMode = UIView.ContentMode.scaleAspectFill
        return cell
    }
    
    func collectionView(_ collectionView: UICollectionView, layout collectionViewLayout: UICollectionViewLayout, minimumLineSpacingForSectionAt section: Int) -> CGFloat {
        return 2.0
    }
}
