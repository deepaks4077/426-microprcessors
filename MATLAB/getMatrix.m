function mtr = getMatrix(filename)
matrix = csvread(filename);
matrix = matrix([1:500],:);
matrixSize = size(matrix,1);
Ones = double(ones(matrixSize,1));
mtr = [matrix,Ones];
end