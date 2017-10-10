#import "videoEncode.h"

- (void) videoWriteTest {
    NSString* newstr = [NSString stringWithFormat:@"yangpian001.jpg"];
    NSString* filepath = [[NSBundle mainBundle] pathForResource:newstr ofType:nil];
    UIImage* images = [UIImage imageWithContentsOfFile:filepath];
    
    NSString *path = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES).firstObject;
    NSFileManager *manager = [NSFileManager defaultManager];
    path = [path stringByAppendingPathComponent:@"zzz"];
    [manager createDirectoryAtPath:path withIntermediateDirectories:YES attributes:nil error:nil];
    path = [path stringByAppendingPathComponent:@"ttt.mp4"];
    
    // Init a Writer
    NSError *err = nil;
    
    AVAssetWriter *videoWriter = [[AVAssetWriter alloc] initWithURL:[NSURL fileURLWithPath:path]
                                                           fileType:AVFileTypeMPEG4
                                                              error:&err];
    NSParameterAssert(videoWriter);
    
    NSDictionary *videoSetting = [NSDictionary dictionaryWithObjectsAndKeys:
                                  AVVideoCodecH264, AVVideoCodecKey,
                                  [NSNumber numberWithInt:640], AVVideoWidthKey,
                                  [NSNumber numberWithInt:480], AVVideoHeightKey,
                                  nil];
    
    AVAssetWriterInput *writerInput = [AVAssetWriterInput assetWriterInputWithMediaType:AVMediaTypeVideo
                                                                         outputSettings:videoSetting];
    
    AVAssetWriterInputPixelBufferAdaptor *adapter =
    [AVAssetWriterInputPixelBufferAdaptor assetWriterInputPixelBufferAdaptorWithAssetWriterInput:writerInput
                                                                     sourcePixelBufferAttributes:nil];
    
    NSParameterAssert(writerInput);
    NSParameterAssert([videoWriter canAddInput:writerInput]);
    [videoWriter addInput:writerInput];
    
    // Start a session
    [videoWriter startWriting];
    [videoWriter startSessionAtSourceTime:kCMTimeZero];
    
    // Write data
    for (int i = 0; i < 30; ++i) {
        [adapter appendPixelBuffer:[self pixelBufferFromCGImage:images.CGImage] withPresentationTime:CMTimeMake(i, 30)];
    }
    
    // Finish session
    [writerInput markAsFinished];
    [videoWriter finishWritingWithCompletionHandler:^(void){NSLog(@"Finished");}];
}

- (CVPixelBufferRef) pixelBufferFromCGImage: (CGImageRef) image
{
    // This again was just our utility class for the height & width of the
    // incoming video (640 height x 480 width)

    int height = 640;
    int width = 480;
    
    NSDictionary *options = [NSDictionary dictionaryWithObjectsAndKeys:
                             [NSNumber numberWithBool:YES], kCVPixelBufferCGImageCompatibilityKey,
                             [NSNumber numberWithBool:YES], kCVPixelBufferCGBitmapContextCompatibilityKey,
                             nil];
    CVPixelBufferRef pxbuffer = NULL;
    
    CVReturn status = CVPixelBufferCreate(kCFAllocatorDefault, width,
                                          height, kCVPixelFormatType_32ARGB, (__bridge CFDictionaryRef) options,
                                          &pxbuffer);
    
    NSParameterAssert(status == kCVReturnSuccess && pxbuffer != NULL);
    
    CVPixelBufferLockBaseAddress(pxbuffer, 0);
    void *pxdata = CVPixelBufferGetBaseAddress(pxbuffer);
    NSParameterAssert(pxdata != NULL);
    
    CGColorSpaceRef rgbColorSpace = CGColorSpaceCreateDeviceRGB();
    
    CGContextRef context = CGBitmapContextCreate(pxdata, width,
                                                 height, 8, 4*width, rgbColorSpace,
                                                 kCGImageAlphaNoneSkipFirst);
    NSParameterAssert(context);
    CGContextConcatCTM(context, CGAffineTransformMakeRotation(0));
    CGContextDrawImage(context, CGRectMake(0, 0, CGImageGetWidth(image),
                                           CGImageGetHeight(image)), image);
    CGColorSpaceRelease(rgbColorSpace);
    CGContextRelease(context);
    
    CVPixelBufferUnlockBaseAddress(pxbuffer, 0);
    
    return pxbuffer;
}
