 var myRoom;
var fileUploadDataValue;
var files;
var downloadedFile;
var downloadFailed;
function passToken(token,fileObject) {
        EnxRtc.Logger.info(token);
        myRoom = EnxRtc.EnxRoom({"token":token});
        myRoom.setFsEndPoint(fileObject);
                window.location = 'iosWebViewLoaded:';
                   myTestFtMobile = () => {
                       var x = document.createElement("INPUT");
                       x.setAttribute("type", "file");
//                       x.setAttribute("multiple",false);
//                       x.setAttribute("name",'file')
                       x.setAttribute("id", 'files');
                       var elToAppend = document.body;
                       elToAppend.appendChild(x);
                       function handleFileSelect(evt) {
                           files = evt.target.files; // FileList object
                           if(files.length > 1){
                               window.location = 'iosMultipleFileError:';
                               return;
                           }
                           myRoom.filesToUpload = files;
                           window.location = 'iosFileUpload:';
                           for (var i = 0, f; f = files[i]; i++) {
                           }
                       }
                document.getElementById('files').addEventListener('change', handleFileSelect, false);
                    addCancelBtn();
                document.getElementById('files').addEventListener("click", function (evt) {
                       window.location = 'iosChoseFile:';
                       evt.target.value = null ;

                       }, false);
                   }
             myTestFtMobile();
        EnxRtc.Logger.info(myRoom);
        }
    function fileSelectionDone(){
        var sFile  = {
            'lastModified'     : files[0].lastModified,
            'lastModifiedDate' : files[0].lastModifiedDate,
            'name'             : files[0].name,
            'size'             : files[0].size,
            'type'             : files[0].type
        };
        return JSON.stringify(sFile);
    }
     function sendFilesToMobile(){
         var callback =(res)=> {
                     fileUploadDataValue = (JSON.stringify(res))
                     window.location = 'iosFileReady:';
            };
         EnxRtc.Logger.info('files to upload are' + 'IOS' + JSON.stringify(myRoom.filesToUpload));
         myRoom.sendFiles(myRoom.filesToUpload,options={"isMobile":true},callback);
        }
        function updateSendFiletoIOS(){
               return fileUploadDataValue;
        }
         function downloadFile(data){
            EnxRtc.Logger.info(' file to download' + JSON.stringify(data));
            myRoom.mobileSetAvailableFile(data, function callback(res)
            {
             myRoom.recvFiles(0,options={"isMobile":true}, function callback(res){
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
    function fileDownloadedIOS(){
        return downloadedFile;
    }
    function fileDownloadFailedIOS(){
        return downloadFailed;
    }
         function addCancelBtn() {
            //alert('btn... called..');
            var cancelBtn = document.createElement('button');
            cancelBtn.setAttribute('id', 'cancel-btn');
            cancelBtn.setAttribute('class', 'cancel-btn');
            cancelBtn.innerHTML = 'X';
             cancelBtn.style.float = 'right';
            cancelBtn.addEventListener('click', function (e){
                //alert('event called..')
                window.location = 'iosCancelTap:';
            })
            document.body.append(cancelBtn);
        }
