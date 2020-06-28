# Multi-Party RTC: A Sample iOS App with EnableX iOS Toolkit

This is a Sample iOS App demonstrates the use of EnableX (https://www.enablex.io) platform Server APIs and iOS Toolkit.  It allows developers to ramp up on app development by hosting on their own devices. 

This App creates a virtual Room on the fly  hosted on the Enablex platform using REST calls and uses the Room credentials (i.e. Room Id) to connect to the virtual Room as a Moderator or Participant using a mobile client.  The same Room credentials can be shared with others to join the same virtual Room to carry out a RTC (Real Time Communication) session. 

> EnableX Developer Center: https://developer.enablex.io/


## 1. How to get started

### 1.1 Pre-Requisites

#### 1.1.1 App Id and App Key 

* Register with EnableX [https://www.enablex.io] 
* Login to the EnableX Portal
* Create your Application Key
* Get your App ID and App Key delivered to your Email


#### 1.1.2 Sample iOS Client 

* Clone or download this Repository [https://github.com/EnableX/Multiparty-Video-Chat-Application-Sample-for-IOS.git] 


#### 1.1.3 Test Application Server

You need to setup an Application Server to provision Web Service API for your iOS Application to communicate enabling Video Session. 

To help you to try our iOS Application quickly, without having to setup Applciation Server, the Application is shipped pre-configured with EnableX hosted Application Server i.e. https://demo.enablex.io. 

Our Application Server restricts a single Session Duation to 10 minutes, and allows 1 moderator and not more than 3 Participant in a Session.

Once you tried EnableX iOS Sample Application, you may need to setup your own  Application Server and verify your Application to work with your Application Server.  More on this, read Point 2 later in the Document.

#### 1.1.4 Configure iOS Client 

* Open the App
* Go to VCXConstant.swift, it's reads- 

``` 
 /* To try the App with Enablex Hosted Service you need to set the kTry = true
    When you setup your own Application Service, set kTry = false */
    
    let kTry = true

 /* Your Web Service Host URL. Keet the defined host when kTry = true */
    
    let kBasedURL = "https://demo.enablex.io/"
     
 /* Your Application Credential required to try with EnableX Hosted Service
    When you setup your own Application Service, remove these */
    
    let kAppId    = ""
    let kAppkey   = ""
 
 ```
### 1.2 Test

#### 1.2.1 Open the App

* Open the App in your Device. You get a form to enter Credentials i.e. Name & Room Id.
* You need to create a Room by clicking the "Create Room" button.
* Once the Room Id is created, you can use it and share with others to connect to the Virtual Room to carry out a RTC Session either as a Moderator or a Participant (Choose applicable Role in the Form).

Note: Only one user with Moderator Role allowed to connect to a Virtual Room while trying with EnableX Hosted Service. Your Own Application Server may allow upto 5 Moderators. 


  
## 2 Setup Your Own Application Server

You may need to setup your own Application Server after you tried the Sample Application with EnableX hosted Server. We have differnt variant of Appliciation Server Sample Code, pick one in your preferred language and follow instructions given in respective README.md file.

*NodeJS: [https://github.com/EnableX/Video-Conferencing-Open-Source-Web-Application-Sample.git]
*PHP: [https://github.com/EnableX/Group-Video-Call-Conferencing-Sample-Application-in-PHP]

Note the following:

* You need to use App ID and App Key to run this Service.
* Your iOS Client End Point needs to connect to this Service to create Virtual Room and Create Token to join session.
* Application Server is created using EnableX Server API, a Rest API Service helps in provisioning, session access and pos-session reporting.  

To know more about Server API, go to:
https://developer.enablex.io/latest/server-api/

## 3 iOS Toolkit

This Sample Applcation uses EnableX iOS Toolkit to communicate with EnableX Servers to initiate and manage Real Time Communications. You might need to update your Application with latest version of EnableX iOS Toolkit time as and when a new release is avaialble.  

* Documentation: https://developer.enablex.io/latest/client-api/ios-toolkit/
* Download: https://developer.enablex.io/resources/downloads/#ios-toolkit


## 4 Demo

EnableX provides hosted Vemo Application of different use-case for you to try out.

1. Try a quick Video Call: https://demo.enablex.io
2. Try Apps on Demo Zone: https://portal.enablex.io/demo-zone/
3. Try Meeting & Webinar:  https://www.enablex.io/ucaas/

