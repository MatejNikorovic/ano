# Practical 04

## [Fourier transform](https://en.wikipedia.org/wiki/Fourier_transform) 

```
Mat computeFourierTransform(Mat& image){
   Mat paddedImage;
   int newWidth = getOptimalDFTSize(image.cols);
   int newHeight = getOptimalDFTSize(image.rows);
   copyMakeBorder(image, paddedImage, 0, newHeight - image.rows, 0, newWidth - image.cols, BORDER_CONSTANT, Scalar::all(0));
   Mat parts[] = {Mat_<float>(paddedImage), Mat(paddedImage.size(), CV_32F, Scalar::all(0))};
   Mat output;
   merge(parts, 2, output);
   dft(output, output, DFT_COMPLEX_OUTPUT);
   return output;
}
```

## Complex image conversion to `log` scale

```
Mat complexToLogScale(Mat& complex){
    Mat parts[2];
    split(complex, parts);
    Mat distances;
    magnitude(parts[0], parts[1], distances);
    distances += Scalar::all(1);
    log(distances, distances);
    normalize(distances, distances, 1, 0, NORM_INF);
	return distances;
}
```

## Inverse fourier transform

```
Mat computeInverseFourierTransform(Mat& complex){
    Mat result, output;
    idft(complex, result);
    Mat parts[2];
    split(result, parts);
    magnitude(parts[0], parts[1], output);
    normalize(output, output, 0, 1, NORM_MINMAX);
    return output;
}
```
