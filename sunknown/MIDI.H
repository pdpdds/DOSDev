enum play_state { MPS_BEGIN, MPS_PLAYING, MPS_DONE };

struct track {
    unsigned char huge *data;
    unsigned char huge *playp;
    unsigned long position;
    unsigned long length;
    unsigned long play_time;
    unsigned char status;
    enum play_state play_state;
    struct track *next_track;
};

/* definitions for MIDI file parsing code */
struct MIDILIB {
    int (*Mf_getc)(void);
    void (*Mf_error)(char *);
    void (*Mf_header)(int, int, int);
    void (*Mp_reset_tick_counter)(void);
    void (*Mp_set_tempo)(unsigned long, short);
    int (*Mp_wait_for)(unsigned long);
    void (*Mp_init_timers)(void);
    void (*Mp_cleanup_timers)(void);
    int (*Mp_reset_midi)(void);
    int (*Mp_note_on)(int, int, int);
    void (*Mp_note_off)(int, int, int);
    void (*Mp_pressure)(int, int, int);
    void (*Mp_parameter)(int, int, int);
    void (*Mp_pitch_bend)(int, int, int);
    void (*Mp_program)(int, int);
    void (*Mp_chanpressure)(int, int, int);
    int (*Mp_init_hardware)(struct MIDILIB *);
    void (*Mp_cleanup)(void);
    struct track *Mp_tracks;
    int tracknum;
    int Mp_master_volume;
    int Mf_nomerge;
    long Mf_toberead;
    unsigned long Mp_next_event_time;
};

struct header_type {
    short format;
    short ntrks;
    union {
	short quarter_note;
	struct{
	    char format;
	    char resolution;
	} smpte;
    } division;
};


/* MIDI status commands most significant bit is 1 */
#define note_off         	0x80
#define note_on          	0x90
#define poly_aftertouch  	0xa0
#define control_change    	0xb0
#define program_chng     	0xc0
#define channel_aftertouch      0xd0
#define pitch_wheel      	0xe0
#define system_exclusive      	0xf0
#define sysex_continue      	0xf7
#define delay_packet	 	(1111)

/* 7 bit controllers */
#define C_damper_pedal          0x40
#define C_portamento	        0x41 	
#define C_sostenuto	        0x42
#define C_soft_pedal	        0x43
#define C_general_4             0x44
#define	C_hold_2		0x45
#define	C_general_5	        0x50
#define	C_general_6	        0x51
#define C_general_7	        0x52
#define C_general_8	        0x53
#define C_tremolo_depth	        0x5c
#define C_chorus_depth	        0x5d
#define	C_detune		0x5e
#define C_phaser_depth	        0x5f

/* parameter values */
#define data_inc	        0x60
#define data_dec	        0x61

/* parameter selection */
#define non_reg_lsb	        0x62
#define non_reg_msb	        0x63
#define reg_lsb		        0x64
#define reg_msb		        0x65

/* Standard MIDI Files meta event definitions */
#define	MT_meta_event		0xff
#define	MT_sequence_number 	0x00
#define	MT_text_event		0x01
#define MT_copyright_notice 	0x02
#define MT_sequence_name    	0x03
#define MT_instrument_name 	0x04
#define MT_lyric	        0x05
#define MT_marker		0x06
#define	MT_cue_point		0x07
#define MT_channel_prefix	0x20
#define	MT_end_of_track		0x2f
#define	MT_set_tempo		0x51
#define	MT_smpte_offset		0x54
#define	MT_time_signature	0x58
#define	MT_key_signature	0x59
#define	MT_sequencer_specific	0x74

struct MIDILIB *mfinit(void (*errorfunc)(char *));
void mfclose(struct MIDILIB **ml);
void mfread(struct MIDILIB *ml);
void mfcleanup(struct MIDILIB *ml);
int midi_getc(struct track *track);
void midi_out(struct MIDILIB *ml);
void midi_get_next_time(struct MIDILIB *ml, struct track *track);
void midi_error(char *fmt, ...);
void midi_message(char *fmt, ...);
