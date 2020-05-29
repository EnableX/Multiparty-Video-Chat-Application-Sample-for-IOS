# Multi-Party RTC: A Sample iOS App with EnableX iOS Toolkit

This is a Sample iOS App demonstrates the use of EnableX (https://www.enablex.io) platform Server APIs and iOS Toolkit.  It allows developers to ramp up on app development by hosting on their own devices. 

This App creates a virtual Room on the fly  hosted on the Enablex platform using REST calls and uses the Room credentials (i.e. Room Id) to connect to the virtual Room as a Moderator or Participant using a mobile client.  The same Room credentials can be shared with others to join the same virtual Room to carry out a RTC (Real Time Communication) session. 

> EnableX Developer Center: https://developer.enablex.io/


## 1. How to get started

### 1.1 Pre-Requisites

#### 1.1.1 App Id and App Key 

* Register with EnableX [https://www.enablex.io] 
* Create your Application
* Get your App ID and App Key delivered to your Email


#### 1.1.2 Sample iOS Client 

* Clone or download this Repository [https://github.com/EnableX/Multiparty-Video-Chat-Application-Sample-for-IOS.git] 


#### 1.1.3 Sample App Server 

* Clone or download this Repository [https://github.com/EnableX/Video-Conferencing-Open-Source-Web-Application-Sample.git] & follow the steps further 
* You need to use App ID and App Key to run this Service. 
* Your iOS Client End Point needs to connect to this Service to create Virtual Room.
* Follow README file of this Repository to setup the Service.


#### 1.1.4 Configure iOS Client 

* Open the App
* Go to VCXConstant.swift and change the following:
``` 
 let userName = "USERNAME"  /* HTTP Basic Auth Username of App Server */
 let password = "PASSWORD"  /* HTTP Basic Auth Password of App Server */
 let kBaseURL = "FQDN"      /* FQDN of of App Server */
 ```
 
 Note: The distributable comes with demo username and password for the Service. 

### 1.2 Test

#### 1.2.1 Open the App

* Open the App in your Device. You get a form to enter Credentials i.e. Name & Room Id.
* You need to create a Room by clicking the "Create Room" button.
* Once the Room Id is created, you can use it and share with others to connect to the Virtual Room to carry out a RTC Session either as a Moderator or a Participant (Choose applicable Role in the Form).

Note: Only one user with Moderator Role allowed to connect to a Virtual Room.


  
## 2 Server API

EnableX Server API is a Rest API service meant to be called from Partners' Application Server to provision video enabled 
meeting rooms. API Access is given to each Application through the assigned App ID and App Key. So, the App ID and App Key 
are to be used as Username and Password respectively to pass as HTTP Basic Authentication header to access Server API.
 
For this application, the following Server API calls are used: 
* https://developer.enablex.io/latest/server-api/rooms-route/#get-rooms - To get list of Rooms
* https://developer.enablex.io/latest/server-api/rooms-route/#get-room-info - To get information of the given Room
* https://developer.enablex.io/latest/server-api/rooms-route/#create-token - To create Token for the given Room

To know more about Server API, go to:
https://developer.enablex.io/latest/server-api/


## 3 iOS Toolkit

iOS App to use iOS Toolkit to communicate with EnableX Servers to initiate and manage Real Time Communications.  

* Documentation: https://developer.enablex.io/latest/client-api/ios-toolkit/
* Download: https://developer.enablex.io/resources/downloads/#ios-toolkit


## 4 Application Walk-through

### 4.1 Create Token

We create a Token for a Room Id to get connected to EnableX Platform to connect to the Virtual Room to carry out a RTC Session.

To create Token, we make use of Server API. Refer following documentation:
https://developer.enablex.io/latest/server-api/rooms-route/#create-token


### 4.2 Connect to a Room, Initiate & Publish Stream

We use the Token to get connected to the Virtual Room. Once connected, we intiate local stream and publish into the room. Refer following documentation for this process:
https://developer.enablex.io/latest/client-api/ios-toolkit/enxrtc/


### 4.3 Play Stream

We play the Stream into EnxPlayerView Object.
``` 
let streamView = EnxPlayerView(frame: CGRect) 
self.view.addSubview(streamView) 
localStream.attachRenderer(streamView) 
  ```
More on Player: https://developer.enablex.io/latest/client-api/ios-toolkit/basic-functions/#play-stream

### 4.4 Handle Server Events

EnableX Platform will emit back many events related to the ongoing RTC Session as and when they occur implicitly or explicitly as a result of user interaction. We use delegates of handle all such events.

``` 
/* Example of Delegates */

/* Delegate: didConnect 
Handles successful connection to the Virtual Room */ 

func room(_ room: EnxRoom?, didConnect roomMetadata: [AnyHashable : Any]?) { 
    /* You may initiate and publish your stream here */
} 


/* Delegate: didError
 Error handler when room connection fails */
 
func room(_ room: EnxRoom?, didError reason: String?) { 

} 

 
/* Delegate: didAddedStream
 To handle any new stream added to the Virtual Room */
 
func room(_ room: EnxRoom?, didAddedStream stream: EnxStream?) { 
    /* Subscribe Remote Stream */
} 

/* Delegate: activeTalkerList
 To handle any time Active Talker list is updated */
  
func room(_ room: EnxRoom?, activeTalkerList Data: [Any]?) { 
    /* Handle Stream Players */
}
```

## 5 Demo

Visit Demo Zone (https://portal.enablex.io/demo-zone/) to request a Guided Demo or Demo Access to different type of application available there.

You may also try our Video Meeting and Webinar solutions here: https://enablex.io/vcfree
