paths = {...
    'C:\Users\gd355\Source\temporal_eyecoding\core\temp\text\full_half\img', ...
    'C:\Users\gd355\Source\temporal_eyecoding\core\temp\text\CLF.RT_half\img', ...
    'C:\Users\gd355\Source\temporal_eyecoding\core\temp\120\text\full_half\img', ...
    'C:\Users\gd355\Source\temporal_eyecoding\core\temp\120\text\CLF.RT_half\img',...
    'C:\Users\gd355\Source\temporal_eyecoding\core\temp\120\circ\full_half\img', ...
    'C:\Users\gd355\Source\temporal_eyecoding\core\temp\120\circ\CLF.RT_half\img'};

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


c = 49;
while 1
    
disp('requesting a video');
    if c == 49
        fwrite(networkSocket, ['P' paths{1} '#'],'uint8');
    elseif c == 50
        fwrite(networkSocket, ['P' paths{2} '#'],'uint8');
    elseif c == 51
        fwrite(networkSocket, ['P' paths{3} '#'],'uint8');
    elseif c == 52
        fwrite(networkSocket, ['P' paths{4} '#'],'uint8');
    elseif c == 53
        fwrite(networkSocket, ['P' paths{5} '#'],'uint8');
    elseif c == 54
        fwrite(networkSocket, ['P' paths{6} '#'],'uint8');
    end
    
    while (networkSocket.BytesAvailable == 0)
        pause( 0.5 );
    end
    c = fread(networkSocket, 1);      
    disp(c)
end
fclose(networkSocket);