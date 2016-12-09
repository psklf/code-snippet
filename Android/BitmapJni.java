@Override
protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);

    setContentView(R.layout.activity_main);

    ImageView imageView;
    imageView = (ImageView) findViewById(R.id.bmp_iv);

    // Init
    RgbStitcher stitcher = new RgbStitcher();
    stitcher.InitRgbStitch(
            "/data/local/tmp/param480.bin");

    long startTime = System.currentTimeMillis();

    // bitmap
    Bitmap bmp = readBmp();
    int width = bmp.getWidth();
    int height = bmp.getHeight();
    int bytes = bmp.getByteCount();
    Bitmap.Config type = bmp.getConfig();

    ByteBuffer buf = ByteBuffer.allocate(bytes);
    bmp.copyPixelsToBuffer(buf);

    byte[] byteArray = buf.array();

    boolean stitchFinish = stitcher.RgbStitchProcess(byteArray, width, height);

    if (stitchFinish) {
        long stitch_s_t = System.currentTimeMillis();
        Log.i(TAG, "all stitch time " + (stitch_s_t - startTime));

        // use Bitmap.Config.ARGB_8888 instead of type is OK
        Bitmap stitchBmp = Bitmap.createBitmap(width, height, type);

        stitchBmp.copyPixelsFromBuffer(ByteBuffer.wrap(byteArray));

        imageView.setImageBitmap(stitchBmp);
    }

}

/**
 * @return bmp
 * for test
 */
private Bitmap readBmp() {
    BitmapFactory.Options options = new BitmapFactory.Options();
    options.inScaled = false;

    return BitmapFactory.decodeResource(getResources(), R.drawable.bmp_013, options);
}
