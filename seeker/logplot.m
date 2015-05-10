N=500;
data=dlmread("log.csv"," ");
nsets=(size(data)(2)/N);
data2=zeros(nsets,N);
for a=1:nsets
	data2(a,1:N)=data(N*(a-1)+1:N*(a-1)+N);
endfor;

plot((data2'));


