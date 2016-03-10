Y_z_down = double([0,0,1]);
Y_z_up = double([0,0,-1]);
Y_y_down = double([0,1,0]);
Y_y_up = double([0,-1,0]);
Y_x_down = double([1,0,0]);
Y_x_up = double([-1,0,0]);

z_down = getMatrix('Z_down.csv');
Y_z_down = repmat(Y_z_down,size(z_down,1),1);

z_up = getMatrix('Z_up.csv');
Y_z_up = repmat(Y_z_up,size(z_up,1),1);

y_down = getMatrix('Y_down.csv');
Y_y_down = repmat(Y_y_down,size(y_down,1),1);

y_up = getMatrix('Y_up.csv');
Y_y_up= repmat(Y_y_up,size(y_up,1),1);

x_down = getMatrix('X_down.csv');
Y_x_down = repmat(Y_x_down,size(x_down,1),1);

x_up = getMatrix('X_up.csv');
Y_x_up= repmat(Y_x_up,size(x_up,1),1);

Y = [Y_x_down;Y_x_up;Y_y_down;Y_y_up;Y_z_down;Y_z_up];
W = [x_down;x_up;y_down;y_up;z_down;z_up];

calibratedMatrix = calibration(W,Y);
disp(calibratedMatrix);
