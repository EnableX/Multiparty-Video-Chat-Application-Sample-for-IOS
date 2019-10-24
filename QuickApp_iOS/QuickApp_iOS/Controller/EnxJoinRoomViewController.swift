//
//  EnxJoinRoomViewController.swift
//  QuickApp_iOS
//
//  Created by Daljeet Singh on 04/12/18.
//  Copyright © 2018 Daljeet Singh. All rights reserved.
//

import Foundation

import UIKit
import SVProgressHUD
import AVFoundation

class EnxJoinRoomViewController: UIViewController  {
    
    @IBOutlet weak var participant: UIButton!
    @IBOutlet weak var topView: UIView!
    @IBOutlet weak var roomNameTxt: UITextField!
    @IBOutlet weak var nameTxt: UITextField!
    @IBOutlet weak var joinBtn: UIButton!
    @IBOutlet weak var stackView: UIView!
    @IBOutlet weak var moderator: UIButton!
    @IBOutlet weak var createRoom: UIButton!
    var isModerator : Bool! = false
    
    @IBOutlet weak var shareBtn: UIButton!
    override func viewDidLoad() {
        super.viewDidLoad()
        getPermissions()
        self.prepareView()
        // Do any additional setup after loading the view, typically from a nib.
    }
    func getPermissions() {
        let vStatus = AVCaptureDevice.authorizationStatus(for: .video)
        if(vStatus == AVAuthorizationStatus.notDetermined){
            AVCaptureDevice.requestAccess(for: .video, completionHandler: { (granted: Bool) in
            })
        }
        let aStatus = AVCaptureDevice.authorizationStatus(for: .audio)
        if(aStatus == AVAuthorizationStatus.notDetermined){
            AVCaptureDevice.requestAccess(for: .audio, completionHandler: { (granted: Bool) in
            })
        }
    }
    // MARK: - prepareView
    /**
     adjust mainView layer conrnerRadius
     adjust joinBtn layer conrnerRadius
     adjust createRoom layer conrnerRadius
     adjust topView layer conrnerRadius
     check its Room All ready Created or not
     **/
    private func prepareView(){
        stackView.layer.cornerRadius = 8.0
        joinBtn.layer.cornerRadius = 8.0
        topView.round(corners: [.topLeft, .topRight], radius: 8.0)
        moderator.isSelected = true
        createRoom.layer.cornerRadius = 8.0
        if UserDefaults.standard.string(forKey: "QRood_Id") != nil{
            let userdef = UserDefaults.standard
            self.roomNameTxt.text = userdef.string(forKey: "QRood_Id")
            shareBtn.isHidden = false
        }
    }
    // MARK: - getFromuserDef
    /**
     This method help to get all parameter which we saved in userDefault
     Input parameter :- EnxRoomInfoModel
     **/
    func getFromuserDef() -> EnxRoomInfoModel{
        let roomdataModel = EnxRoomInfoModel()
        roomdataModel.room_id = self.roomNameTxt.text
        roomdataModel.participantName = self.nameTxt.text
        if UserDefaults.standard.string(forKey: "Qmode") != nil{
            let userdef = UserDefaults.standard
            roomdataModel.mode = userdef.string(forKey: "Qmode")
        }
        return roomdataModel
    }
    
    // MARK: - Join Button Event
    /**
     Validate  maindatory Filed should not empty
     Show Loader
     Call Rest Service to join Room with Required Information.
     **/
    @IBAction func clickToJoinRoom(_ sender: Any) {
        guard let nameStr = nameTxt.text?.trimmingCharacters(in: .whitespaces) ,!nameStr.isEmpty else{
            self.showAleartView(message: "Please enter name", andTitles: "OK")
            return}
        guard let roomNameStr = roomNameTxt.text?.trimmingCharacters(in: .whitespaces) , !roomNameStr.isEmpty else {
            self.showAleartView(message: "Please enter room Id", andTitles: "OK")
            return}
        guard EnxNetworkManager.isReachable() else {
            self.showAleartView(message:"Kindly check your Network Connection", andTitles: "OK")
            return
        }
        SVProgressHUD.show()
        EnxServicesClass.fetchRoomInfoWithRoomId(roomId :roomNameStr ,completion:{roomModel  in
            DispatchQueue.main.async {
                //Success Response from server
                if roomModel.room_id != nil{
                    if self.isModerator{
                        roomModel.role = "moderator"
                    }else{
                        roomModel.role = "participant"
                    }
                    roomModel.participantName = nameStr
                    self.performSegue(withIdentifier: "ConferenceView", sender: roomModel)
                }
                    //Handeling server giving no error but due to wrong PIN room not available
                else if roomModel.isRoomFlag == false && roomModel.error == nil {
                    self.showAleartView(message:"Room not found", andTitles: "OK")
                }
                    //Handeling server error
                else{
                    print(roomModel.error)
                    self.showAleartView(message:roomModel.error, andTitles: "OK")
                }
                SVProgressHUD.dismiss()
            }
        })
    }
    // MARK: - SegueEvent
    /**
     here getting refrence to next moving controll and passing requirade parameter
     Input parameter :- UIStoryboardSegue andAny
     Return parameter :- Nil
     **/
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        if (segue.identifier == "ConferenceView") {
            let parameters = ["video" : true , "audio" : true , "chat" : true]
            let confrenceVC = segue.destination as! EnxConfrenceViewController
            confrenceVC.roomInfo = (sender as! EnxRoomInfoModel)
            confrenceVC.param = parameters
        }
        else if (segue.identifier == "popOverView"){
            let popoverViewController = segue.destination as! EnxMenuViewController
            popoverViewController.delegate = self
            popoverViewController.modalPresentationStyle = .popover
            popoverViewController.presentationController?.delegate = self
            popoverViewController.popoverPresentationController?.sourceView = shareBtn
            popoverViewController.popoverPresentationController?.sourceRect  = CGRect(x: 0, y: 0, width: shareBtn.frame.size.width, height: shareBtn.frame.size.height)
            popoverViewController.preferredContentSize = CGSize(width: 150, height: 50)
        }
    }
    // MARK: - Show Alert
    /**
     Show Alert Based in requirement.
     Input parameter :- Message and Event name for Alert
     **/
    private func showAleartView(message : String, andTitles : String){
        let alert = UIAlertController(title: " ", message: message, preferredStyle: UIAlertController.Style.alert)
        alert.addAction(UIAlertAction(title: andTitles, style: .default, handler: nil))
        self.present(alert, animated: true, completion: nil)
    }
    // MARK: - Select Moderator/Participant
    /**
     Change User Either Moderator/Participant
     Input parameter :- UIButton
     **/
    @IBAction func selecteRoleType(_ sender: UIButton) {
        if sender == moderator{
            isModerator = true
            participant.isSelected = false
            moderator.isSelected = true
        }
        else if sender == participant {
            isModerator = false
            participant.isSelected = true
            moderator.isSelected = false
        }
    }
    // MARK: - saveTOuserDef
    /**
     This method help to save all parameter which we get from room service
     Input parameter :- EnxRoomInfoModel
     **/
    private func saveTOuserDef(roomInfo:EnxRoomInfoModel){
        let usdef = UserDefaults.standard
        usdef.set(roomInfo.room_id, forKey: "QRood_Id")
        usdef.set(roomInfo.mode, forKey: "Qmode")
        usdef.synchronize()
    }
    // MARK: - Create Room
    /**
     Call to create Room
     Input parameter :- Any
     **/
    @IBAction func createRoomEvent(_ sender: Any) {
        guard EnxNetworkManager.isReachable() else {
            self.showAleartView(message:"Kindly check your Network Connection", andTitles: "OK")
            return
        }
        SVProgressHUD.show()
        EnxServicesClass.createRoom(completion:{roomModel  in
            DispatchQueue.main.async {
                //Success Response from server
                if roomModel.room_id != nil{
                    self.roomNameTxt.text = roomModel.room_id
                    self.shareBtn.isHidden = false
                    self.saveTOuserDef(roomInfo: roomModel)
                }
                //Handeling server giving no error but due to wrong PIN room not available
                else if roomModel.isRoomFlag == false && roomModel.error == nil {
                    self.showAleartView(message:"Unable to connect, Kindly try again", andTitles: "OK")
                }
                //Handeling server error
                else{
                    print(roomModel.error)
                    self.showAleartView(message:roomModel.error, andTitles: "OK")
                }
                SVProgressHUD.dismiss()
            }
        })
    }
    // MARK: - shareEvents
    /**
     Here we call event to show option for share Room Id
     Input parameter :- Any
     **/
    @IBAction func shareEvents(_ sender: Any) {
        self.performSegue(withIdentifier: "popOverView", sender: nil)
    }
}
// MARK: - UIPopover controller delegate methods
extension EnxJoinRoomViewController : UIPopoverPresentationControllerDelegate {
    func adaptivePresentationStyle(for controller: UIPresentationController) -> UIModalPresentationStyle {
        return UIModalPresentationStyle.none
    }
}
// MARK: - Call Back Event for EnxMenuView
extension EnxJoinRoomViewController : TableTapEvent{
    
    func tapeventFire(index : Int){
        self.dismiss(animated: true, completion: nil)
        let activity = UIActivityViewController(activityItems: [self], applicationActivities: nil)
        present(activity, animated: true)
    }
    
    
}
// MARK: - Call Back Event for UIActivityItemSource
extension EnxJoinRoomViewController : UIActivityItemSource {
    
    func activityViewControllerPlaceholderItem(_ activityViewController: UIActivityViewController) -> Any {
        return "Sample Message"
    }
    
    func activityViewController(_ activityViewController: UIActivityViewController, itemForActivityType activityType: UIActivity.ActivityType?) -> Any? {
        let roomInfo = getFromuserDef()
        return "Hi, \n" +  roomInfo.participantName! + " has invited you to join room with Room Id -" + roomInfo.room_id!
    }
    func activityViewController(_ activityViewController: UIActivityViewController, subjectForActivityType activityType: UIActivity.ActivityType?) -> String {
        return "Room Invite"
    }
}
// MARK: - Extension for View Property
extension UIView {
    func round(corners: UIRectCorner, radius: CGFloat) {
        let path = UIBezierPath(roundedRect: bounds, byRoundingCorners: corners, cornerRadii: CGSize(width: 5, height: radius))
        let mask = CAShapeLayer()
        mask.path = path.cgPath
        self.layer.mask = mask
    }
}
