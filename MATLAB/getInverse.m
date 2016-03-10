function inverse = getInverse(x)
tmp = inv(x'*x);
inverse = tmp*x';
end