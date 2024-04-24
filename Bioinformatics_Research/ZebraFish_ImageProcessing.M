%files = dir('/vol/home/Documents/w8t1t2 mutant day 7/*.tif');
%files = dir('D:\Matlab\w8t1t2 mutant day 7\*.tif');
files = dir('C:\Users\Documents\w8t1t2 mutant day 7\*.tif');
framework = [];
row = [];
totalsize = [];
totalblue = [];
procenten = [];
cvarray_a = [];
cvarray_b = [];
bluecvarray_a = [];
bluecvarray_b = [];
totalentropies = [];
blueentropies = [];
rementropies = [];
totalmean = [];
totalexmean = [];
totalstd = [];


for i = 1:77
    files(i).name;
    %this_image = readim(['/vol/home/Documents/w8t1t2 mutant day 7/' files(i).name]);
    %this_image = readim(['D:\Matlab\w8t1t2 mutant day 7\' files(i).name]);
    this_image = readim(['C:\Users\Documents\w8t1t2 mutant day 7\' files(i).name]);
    %Smooth first
    %blur = gaussf(this_image, 1, 'best');
    
    R = this_image{1};
    G = this_image{2};
    B = this_image{3};
    
    blur = gaussf(this_image,1,'best');
    t_R = threshold(R);
    t_B = threshold(B);
    t_R = ~t_R;
    m = t_R * t_B;
    new_image = m * blur;
    B_2 = new_image{3};
    rest = closing(B_2,4,'elliptic'); %4
    %rest = opening(rest,10,'elliptic'); %added
    rest = erosion(rest,2,'elliptic');
    rest = closing(rest,6,'elliptic');
    t_Rs = dilation(t_R,2,'elliptic');
    t_Rs = opening(t_Rs,6,'elliptic');
    t_rest = threshold(rest);
    m2 = t_Rs * t_rest;
    Banaan = label(m2,2,0,100000000000000000000);
    ms =  measure(Banaan, [], 'Size');
    sizes = ms.Size;
    labels_bigger_than_mean = find(sizes >= mean(sizes));
    L = length(labels_bigger_than_mean);

    Finale_Banaan = Banaan == labels_bigger_than_mean(1);
    if L > 1
        for j = 2:L
            Finale_Banaan = Finale_Banaan + (labels_bigger_than_mean(j) == Banaan);
        end
    end
        
    %ero_banaan = opening(Finale_Banaan,26,'elliptic');
    %testbanaan = erosion(Finale_Banaan,10,'elliptic');
    %testbanaan = opening(testbanaan,10,'elliptic');
    testbanaan = erosion(Finale_Banaan,10,'elliptic');
    test3 = opening(testbanaan,17,'elliptic');
    last_image = test3 * blur;
    
    %end_banaan = ero_banaan * blur;
    %writeim(test3,['/vol/home/s1298062/Documents/Fruitsalade/new' files(i).name],'TIFF',0,[])
    %writeim(test3,['D:\Matlab\WindowsImagery\new' files(i).name],'TIFF',0,[]);
    writeim(test3,['C:\Users\Documents\WindowsImagery\new' files(i).name],'TIFF',0,[]);
    
    zeroimage = t_B * ~t_B;
    
    if i < 59
        row = cat(1,row,test3);
        if mod(i,15) == 0
            framework = cat(2,framework,row);
            row = [];
        end
        
        if i == 58
            row = cat(1,row,zeroimage);
            row = cat(1,row,zeroimage);
            framework = cat(2,framework,row);
            %imwrite(framework,'/vol/home/Documents/Fruitsalade/newconcat.png');
        end
    end
    
    %Open framework this way: dipshow(framework);
    
%STARTING SIZE AND SHAPE CALCULATIONS:    
    %Calculating Sizes (Entire Segmentated Object):
    filledbchannel = fillholes(t_B,1);
    filledbchannel = opening(filledbchannel,2,'elliptic');
    allobjects = label(filledbchannel,2,0,100000000000000000000);
    allmeasures = measure(allobjects, [], 'Size');
    allsizes = allmeasures.Size;
    temp1 = sum(allsizes);
    %Absolute Total Shape:
    totalsize = cat(2,totalsize,sum(allsizes));
    
    %Calculating Sizes (Blue Objects):
    labeltjes = label(test3,2,0,100000000000000000000);
    measurements = measure(labeltjes, [], 'Size');
    bluesizes = measurements.Size;
    temp2 = sum(bluesizes);
    %Absolute Blue Shape:
    totalblue = cat(2,totalblue,sum(bluesizes));
    
    %Relative Amount of Gen Expression In Percentages:
    peer = temp2 / temp1;
    %procentpeer = peer * 100;
    procenten = cat(2,procenten,peer);
    
    
    %Calculating Convexity (Entire Segmentated Object):
    allcvmsr1 = measure(allobjects,[],{'Convexity'},[],1,0,0);
    allcvmsr2 = measure(allobjects,[],{'Convexity'},[],2,0,0);
    
    allconvexes1 = allcvmsr1.Convexity;
    meanconvexes1 = mean(allconvexes1);
    cvarray_a = cat(2,cvarray_a,meanconvexes1);
    
    allconvexes2 = allcvmsr2.Convexity;
    meanconvexes2 = mean(allconvexes2);
    cvarray_b = cat(2,cvarray_b,meanconvexes2);
    
    
    %Calculating Convexity (Blue Objects):
    bluecvmsr1 = measure(labeltjes,[],{'Convexity'},[],1,0,0);
    bluecvmsr2 = measure(labeltjes,[],{'Convexity'},[],2,0,0);
    
    blueconvexes1 = bluecvmsr1.Convexity;
    meanblueconvexes1 = mean(blueconvexes1);
    bluecvarray_a = cat(2,bluecvarray_a,meanblueconvexes1);
    
    blueconvexes2 = bluecvmsr2.Convexity;
    meanblueconvexes2 = mean(blueconvexes2);
    bluecvarray_b = cat(2,bluecvarray_b,meanblueconvexes2);
    
%TIME FOR SOME TEXTURE CALCULATIONS:
    %Prepare Total Object:
    texturetotalprep = filledbchannel * this_image;
    %greyimage = texturetotalprep{3}; %VRAAG OF DIT MAG
    greyimage = 0.2989 * texturetotalprep{1} + 0.5870 * texturetotalprep{2} + 0.1140 * texturetotalprep{3};
    
    %Calculate Entropy of Total Object:
    totalent = entropy(greyimage);
    totalentropies = cat(2,totalentropies,totalent);
   
    %Calculate Mean of Total Object:
    %allmeanmsr1 = measure(filledbchannel,greyimage,{'Mean'},[],Inf,0,0);
    y = greyimage / 256;
    y = stretch(y);
    x = diphist(y);
    temptmean = mean(x(2:end));
    totalmean = cat(2,totalmean,temptmean);
    %Extra Mean Calculation:
    temptexmean = sum(y) / sum(y > 0);
    totalexmean = cat(2,totalexmean,temptexmean);
    
    %Calculate Standard Deviation of Total Object:
    temptstd = std(x(2:end));
    totalstd = cat(2,totalstd,temptstd);
    
    
    
    %Prepare Blue Objects:
    textureblueprep = test3 * this_image;
    %greyblueimage = textureblueprep{3}; %VRAAG OF DIT MAG
    greyblueimage = 0.2989 * textureblueprep{1} + 0.5870 * textureblueprep{2} + 0.1140 * textureblueprep{3};
    %Calculate Entropy of Total Object:
    blueent = entropy(greyblueimage);
    blueentropies = cat(2,blueentropies,blueent);
    
    %Prepare Remainder Objects: %VRAAG OF DIT OF TOTAL
    remainder = ~test3 * filledbchannel;
    remainder = opening(remainder,10,'elliptic');
    textureremprep = remainder * this_image;
    %greyremimage = textureremprep{3};
    greyremimage = 0.2989 * textureremprep{1} + 0.5870 * textureremprep{2} + 0.1140 * textureremprep{3};
    %Calculate Entropy of Remainder Object:
    rement = entropy(greyremimage);
    rementropies = cat(2,rementropies,rement);
    
    
    
    
end
