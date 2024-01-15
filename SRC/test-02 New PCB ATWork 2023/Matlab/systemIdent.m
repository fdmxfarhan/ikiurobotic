clc;

A = xlsread('50 chirp(0.1 t).xlsx');
input1 = A(:,4);
output1 = A(:,3);

A = xlsread('50 chirp(0.2 t).xlsx');
input2 = A(:,4);
output2 = A(:,3);

A = xlsread('50 chirp(0.5 t).xlsx');
input3 = A(:,4);
output3 = A(:,3);

A = xlsread('50 chirp(0.07t).xlsx');
input4 = A(:,4);
output4 = A(:,3);

A = xlsread('50 chirp(0.7 t).xlsx');
input5 = A(:,4);
output5 = A(:,3);

A = xlsread('50 sin(0.2 t).xlsx');
input6 = A(:,4);
output6 = A(:,3);

A = xlsread('50 sin(0.4 t).xlsx');
input7 = A(:,4);
output7 = A(:,3);

A = xlsread('50 sin(0.8t).xlsx');
input8 = A(:,4);
output8 = A(:,3);

A = xlsread('50 sin(1t).xlsx');
input9 = A(:,4);
output9 = A(:,3);

