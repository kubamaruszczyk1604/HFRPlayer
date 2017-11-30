paths = {...
    'C:\Users\gd355\Documents\temporal_eyecoding\core\temp\test\test', ...
    'C:\Users\gd355\Documents\temporal_eyecoding\core\temp\test2\test'};
activePath = 1;

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

disp('requesting a video"');

c = 0;
while 1
    fwrite(networkSocket, [paths{activePath + 1} '#'],'uint8');
    activePath = 1 - activePath;
    while (networkSocket.BytesAvailable == 0)
        pause( 0.5 );
    end
    c = fread(networkSocket, 1);      
    disp(c)
end
fclose(networkSocket);