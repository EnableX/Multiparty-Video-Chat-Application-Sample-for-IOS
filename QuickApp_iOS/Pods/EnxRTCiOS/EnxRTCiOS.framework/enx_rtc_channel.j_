var myRoom;
var fileUploadDataValue;
var selectedfiles;
var downloadedFile;
var downloadFailed;
var cancleResponse;
var preeCall = null;
var preTestFinished;
var preTestFailed;
var preTestStatus;
var preTestStopped;
var preTestInit;
var flag = true;
var preTestStatusFinished;
function passToken(token,fileObject) {
        EnxRtc.Logger.info(token);
        myRoom = EnxRtc.EnxRoom({"token":token});
        myRoom.setFsEndPoint(fileObject);
        window.location = 'iosWebViewLoaded:';
        myRoom.addEventListener("fs-upload-result", function(event) {
                const evtMsg = event.message;
                switch (evtMsg.messageType) {
                    case "upload-started":
                        selectedfiles = JSON.stringify(evtMsg.response);
                        window.location = 'iosFileUploadStart:';
                            break;
                    case "upload-completed":
                            fileUploadDataValue = JSON.stringify(evtMsg.response);
                            window.location = 'iosFileUploadFinished:';
                                break;
                    case "upload-failed":
                            fileUploadDataValue = JSON.stringify(evtMsg.response);
                            window.location = 'iosFileUploadFailed:';
                                break;
                     default:
                    }
        });
        myRoom.addEventListener("fs-download-result", function(event) {
                    const evtMsg = event.message;
                    switch (evtMsg.messageType) {
                        case "download-started":
                            downloadedFile = JSON.stringify(evtMsg.response);
                            window.location = 'iosFileDownloadStarted:';
                    break;
                  case "download-completed":
                        //alert("download-completed");
                    break;
                  case "download-failed":
                        downloadFailed = JSON.stringify(evtMsg.response);
                        window.location = 'iosFileDownoadFailed:';
                    break;
                  default:
                }
              });
                   myTestFtMobile = () => {
                       var x = document.createElement("INPUT");
                       x.setAttribute("type", "file");
                       x.setAttribute("id", 'files');
                       var elToAppend = document.body;
                       elToAppend.appendChild(x);
                       function handleFileSelect(evt) {
                           var files = evt.target.files;
                           if(files.length > 1){
                               window.location = 'iosMultipleFileError:';
                           }
                           else if(files[0].size === 0){
                              window.location = 'iosCorruptedFileError:';
                           }
                           else{
                               myRoom.filesToUpload = files;
                               sendFilesToMobile();
                           }
                       }
                document.getElementById('files').addEventListener('change', handleFileSelect, false);
                   }
             myTestFtMobile();
            EnxRtc.Logger.info(myRoom);
        }
    function fileSelectionDone(){
        return selectedfiles;
    }
     function sendFilesToMobile(){
         var callback =(res)=> {
            };
         console.log('files to upload are' + 'IOS' + JSON.stringify(myRoom.filesToUpload));
         myRoom.sendFiles(myRoom.filesToUpload,options={"isMobile":true},callback);
        }
        function updateSendFiletoIOS(){
               return fileUploadDataValue;
        }
        function failedSendFiletoIOS(){
                return fileUploadDataValue;
        }
         function downloadFile(data,index){
            EnxRtc.Logger.info(' file to download' + JSON.stringify(data),index);
            myRoom.mobileSetAvailableFile(data, function callback(res)
            {
             myRoom.recvFiles(index,options={"isMobile":true}, function callback(res){
                    if(res.result==0){
                    var reader = new window.FileReader();
                        reader.readAsDataURL(res.response.blob);
                        reader.onloadend = function() {
                        downloadedFile = reader.result;
                        window.location = 'iosFileDownoad:';
                    }
                }
                else{
                        downloadFailed = JSON.stringify(res)
                         window.location = 'iosFileDownoadFailed:';
                    }
            });
            }
            );
         }
    function fileDownloadedInitiatedIOS(){
        return downloadedFile;
    }
    function fileDownloadedIOS(){
        return downloadedFile;
    }
    function fileDownloadFailedIOS(){
        return downloadFailed;
    }
     function cancelUploads(cancelAll,id) {
       if(cancelAll){
           myRoom.cancelUploads(true,"cancel", function callback(res) {
                alert("All file cancle");
           cancleResponse = JSON.stringify(res);
            window.location = 'iosFileUploadCancle:';
       });
       }
       else{
        myRoom.cancelUploads(false, id, function callback(res) {
                cancleResponse = JSON.stringify(res);
                window.location = 'iosFileUploadCancle:';
            });
        }
     }
     function cancelDownloads(cancelAll, id) {
        if(cancelAll){
                myRoom.cancelDownloads(true,"cancel", function callback(res) {
                cancleResponse = JSON.stringify(res);
                window.location = 'iosFileDownloadCancle:';
                });
            }
        else{
                myRoom.cancelDownloads(false, id, function callback(res) {
                cancleResponse = JSON.stringify(res);
                window.location = 'iosFileDownloadCancle:';
                });
            }
     }
    function uploadCancleIOS(){
        return cancleResponse;
    }
    function DownloadCancleIOS(){
        return cancleResponse;
    }

/*Pree Call Test*/
/*
function performPreTestCall(Options){
    try{

        preeCall = EnxRtc.clientDiagnostics(Options);

        preeCall.addEventListener("client-diagnosis-finished", function(event) {
                //preTestFinished = JSON.stringify(event.message);
                //window.location = 'client-diagnosis-finished:';
        });
        preeCall.addEventListener("client-diagnosis-failed", function(event) {
            //preTestFailed = JSON.stringify(event.message);
            //window.location = 'client-diagnosis-failed:';
        });
        preeCall.addEventListener("client-diagnosis-status", function(event) {
            //preTestStatus = JSON.stringify(event.message);
            //window.location = 'client-diagnosis-status:';
        });
        
        preeCall.addEventListener("client-diagnosis-stopped", function(event) {
            //preTestStopped = JSON.stringify(event.message);
            //window.location = 'client-diagnosis-stopped:';
        });
    }catch(err) {
        alert("Method Error");
        console.log("Inside Catch",err.message);
        }
}
function getPremission(){
    
    try{
//                const constraints = {
//                    audio :true,
//                    video: true,
//                };
       
//        if (!window.navigator) {
//            window.navigator = function(){
//                window.location = 'premissionGiven123:';
//            };}
//          window.navigator.getUserMedia(constraints) = function() {
//            webkit.messageHandlers.callbackHandler.postMessage(arguments);
//              preTestInit = arguments;
//              window.location = 'premissionGiven:';
//          }
      
        
        const constraints = {
            audio :true,
            video: true,
        };
//        if (!window.navigator) {window.location = 'No Window and nav found:';}
//          window.navigator.getUserMedia(constraints) = function() {
//              preTestInit = "success";
//              window.location = 'premissionGiven:';
//            webkit.messageHandlers.callbackHandler.postMessage(arguments);
//          }
        window.navigator.mediaDevices.getUserMedia(constraints).then((stream) => {
        if(stream != null){
            preTestInit = "success";
            window.location = 'premissionGiven:';
        }
        else{
            //alert("getUserMedia() is not supported by your browser");
            window.location = 'premissionDeny:';
        }
    });
    }
    catch(err) {
        window.location = 'premissionDeny12:';
//        alert("Method Error");
//        console.log("Inside Catch",err.message);
        }
    
}
function clientDiagnosisFinished(){
    return preTestFinished;
}
function clientDiagnosisfailed(){
    return preTestFailed;
}
function clientDiagnosisStatus(){
    return preTestStatus;
}
function clientDiagnosisStopped(){
    return preTestStopped;
}
function clientDiagnosisInit(){
    return preTestInit;
}
function clientDiagnosisStatusFinished(){
    return preTestStatusFinished;
}
*/
