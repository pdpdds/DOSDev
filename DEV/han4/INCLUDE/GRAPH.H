/* video adapters */
enum
{
   NONE,
   MDA,
   HGC,
   CGA,
   EGA,
   VGA,
   MCGA,
   ET3000,
   ET4000,
   WONDER1,
   WONDER2,
};

/* ROM BIOS video modes */
enum
{
   MODE_320x200x4   = 0x04,
   MODE_640x200x2   = 0x06,
   MODE_320x200x16  = 0x0d,
   MODE_640x200x16,
   MODE_640x350x2,
   MODE_640x350x16,
   MODE_640x480x2,
   MODE_640x480x16,
   MODE_320x200x256
};

/* graphics modes */
enum
{
   HGC_640x400x2_0,		/* -1 means not in graphics mode */
   HGC_640x400x2_1,
   HGC_720x348x2_0,
   HGC_720x348x2_1,
   CGA_640x200x2,
   CGA_320x200x4,
   EGA_640x350x16,
   EGA_320x200x16,
   EGA_640x200x16,
   VGA_640x480x16,
   VGA_640x400x16,
   VGA_640x480x2,
   SVGA_640x480x256,
   SVGA_640x400x256,
   SVGA_800x600x16,
   SVGA_800x600x256,
   SVGA_1024x768x16,
   SVGA_1024x768x256,
   DETECT
};

/* graphics modes for hangul */
enum
{
   INIT_HGC_640x400x2_0,
   INIT_HGC_640x400x2_1,
   INIT_HGC_720x348x2_0,
   INIT_HGC_720x348x2_1,
   INIT_CGA_640x200x2,
   INIT_EGA_640x350x16,
   INIT_VGA_640x480x16,
   INIT_VGA_640x400x16,
   INIT_SVGA_640x480x256,
   INIT_SVGA_640x400x256,
   INIT_SVGA_800x600x16,
   INIT_SVGA_800x600x256,
   INIT_SVGA_1024x768x16,
   INIT_SVGA_1024x768x256
};

/* return value of init_graph() */
enum
{
   NOT_VALID_ADAPTER,
   NOT_VALID_MODE,
   OK_INIT
};

/* is6845 */
int is_6845(int port_addr);
/* ismda */
int is_mda(void);
/* ishgc */
int is_hgc(void);
/* iscga */
int is_cga(void);
/* isega */
int is_ega(void);
/* isvga */
int is_vga(void);
/* iset3000 */
int is_et3000(void);
/* iset4000 */
int is_et4000(void);
/* iswonder */
int is_wonder(void);
/* iswonder1 */
int is_wonder1(void);
/* iswonder2 */
int is_wonder2(void);
/* detadapt */
int detect_adapter(void);
/* detsvga */
int detect_svga(void);

/* getvideo */
int  get_video_mode(void);
/* setvideo */
void set_video_mode(int mode);

/* setgrhgc */
void set_gr_mode_hgc(int mode);
/* setgrcga */
void set_gr_mode_cga(int mode);
/* setgrega */
void set_gr_mode_ega(int mode);
/* setgrvga */
void set_gr_mode_vga(int mode);
/* setgrsv */
void set_gr_mode_svga(int mode);
/* setgr */
void set_gr_mode(int mode);
/* choosegr */
int  choose_gr_mode(void);

/* getstd */
int  get_standard_mode(int gr_adapter);
int  is_avail_mode(int adapter, int mode);

/* initgrap */
int init_graph(int mode);

/* setrgb */
void set_rgb_palette(int color_num, int red, int green, int blue);

extern int org_video_mode, gr_mode, gr_adapter;
extern int byte_per_line;
extern int width_gr, height_gr;
extern int width_text, height_text;
