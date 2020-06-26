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


#### 1.1.3 Test Application Server

You need to setup an Application Server to provision Web Service API for your iOS Application to communicate enabling Video Session. 

To help you to try our iOS Application quickly, without having to setup Applciation Server, the Application is shipped pre-configured with EnableX hosted Application Server i.e. https://try.enablex.io. 

Our Application Server restricts a single Session Duation to 15 minutes, and allows 1 moderator and note more than 3 Participant in a Session.

Once you tried EnableX iOS Sample Application, you may need to setup your own  Application Server. More on this, read Point 2 later in the Document.

#### 1.1.4 Configure iOS Client 

* Open the App
* Go to VCXConstant.swift, it's reads- 

``` 
 let userName = "demo"  /* HTTP Basic Auth Username of App Server */
 let password = "enablex"  /* HTTP Basic Auth Password of App Server */
 let kBaseURL = "https://try.enablex.io/"      /* FQDN of of App Server */
 
 ```
 Note:- So application has pre configuration with "https://try.enablex.io/". When you have setup your own API server, you need to update these confifuration according. 

### 1.2 Test

#### 1.2.1 Open the App

* Open the App in your Device. You get a form to enter Credentials i.e. Name & Room Id.
* You need to create a Room by clicking the "Create Room" button.
* Once the Room Id is created, you can use it and share with others to connect to the Virtual Room to carry out a RTC Session either as a Moderator or a Participant (Choose applicable Role in the Form).

Note: Only one user with Moderator Role allowed to connect to a Virtual Room.


  
## 2 Setup Your Own Application Server

You may need to setup your own Application Server after you tried the Sample Application with EnableX hosted Server. We have differnt variant of Appliciation Server Sample Code, pick one in your preferred language and follow instructions given in respective README.md file.

*NodeJS: [https://github.com/EnableX/Video-Conferencing-Open-Source-Web-Application-Sample.git]
*PHP: [https://github.com/EnableX/Group-Video-Call-Conferencing-Sample-Application-in-PHP]

Note the following:

* You need to use App ID and App Key to run this Service.
* Your Android Client End Point needs to connect to this Service to create Virtual Room and Create Token to join session.
* Application Server is created using EnableX Server API, a Rest API Service helps in provisioning, session access and pos-session reporting.  

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
 
func room(_ room: EnxRoom?, didError reason: [Any]?) { 

} 

 
/* Delegate: didAddedStream
 To handle any new stream added to the Virtual Room */
 
func room(_ room: EnxRoom?, didAddedStream stream: EnxStream?) { 
    /* Subscribe Remote Stream */
} 

/* Delegate: activeTalkerList
 To handle any time Active Talker list is updated */
  
func room(_ room: EnxRoom?, didActiveTalkerList Data: [Any]?) {
    // Handle individual stream and their player
}
Or
func room(_ room: EnxRoom?, didActiveTalkerView view: UIView?) {
// Get complet view for list of player , add this view to any where in your app.
}

Note:- To receive complete view, User need to add parameter as "activeviews" : "view" in "roomInfo" at the time of join room API call. 
```

## 5 Demo

Visit Demo Zone (https://portal.enablex.io/demo-zone/) to request a Guided Demo or Demo Access to different type of application available there.

You may also try our Video Meeting and Webinar solutions here: https://enablex.io/vcfree
