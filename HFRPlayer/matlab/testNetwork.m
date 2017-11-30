disp('attempting to open network port');

networkSocket = tcpip('localhost',30000);
networkSocket.OutputBufferSize = 1024*1024*3*3;
fopen(networkSocket);
disp('successfully opened network port');

disp('Waiting for welcome character');
c = 0;
while( c ~= 'W')
    while (networkSocket.BytesAvailable == 0)
        pause( 0.5 );
    end
    c = fread(networkSocket, 1);      
end
disp('received welcome character');

disp('writing data. Check whether HFRPlayer received the string "Hello world"');
fwrite(networkSocket, 'Hello world#','uint8');

fclose(networkSocket);