function ACC = calibration(inpMatrix,Y)
w = getInverse(inpMatrix);
ACC = double(w*Y);
end