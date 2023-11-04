//
//  PdInstance.h
//  libpd
//
//  Copyright (c) 2022 Dan Wilcox <danomatika@gmail.com>
//
//  For information on usage and redistribution, and for a DISCLAIMER OF ALL
//  WARRANTIES, see the file, "LICENSE.txt," in this distribution.
//

#import "PdBase.h"

/// PdInstance: a separate pd instance
@interface PdInstance : NSObject

#pragma mark Initializing Pd

/// initialize with message queing, safe to call this more than once
/// returns 0 on success or -1 if libpd was already initialized
- (instancetype)init;

/// initialize with or without message queuing, safe to call this more than once
///
/// for lowest latency, this will result in delegate receiver calls from the
/// audio thread directly which will probably require manual dispatch to the
/// main thread for anything UI-related otherwise there can be crashes or
/// exceptions
///
/// note: stops message polling if queue is NO
///
/// returns 0 on success or -1 if libpd was already initialized
- (instancetype)initWithQueue:(BOOL)queue;

/// returns whether pd was initialized with message queuing
@property (nonatomic, readonly) BOOL isQueued;

/// clear the pd search path for abstractions and externals
/// note: this is called when initializing
- (void)clearSearchPath;

/// add a path to the pd search paths
/// relative paths are relative to the current working directory
/// unlike desktop pd, *no* search paths are set by default (ie. extra)
- (void)addToSearchPath:(NSString *)path;

#pragma mark Opening Patches

/// open a patch by filename and parent dir path
/// returns an opaque patch handle pointer or nil on failure
- (void *)openFile:(NSString *)baseName path:(NSString *)pathName;

/// close a patch by patch handle pointer
- (void)closeFile:(void *)x;

/// get the $0 id of the patch handle pointer
/// returns $0 value or 0 if the patch is non-existent
- (int)dollarZeroForFile:(void *)x;

#pragma mark Audio Processing

/// return pd's fixed block size: the number of sample frames per 1 pd tick
+ (int)getBlockSize;

/// initialize audio rendering
/// returns 0 on success
- (int)openAudioWithSampleRate:(int)samplerate
                 inputChannels:(int)inputChannels
				outputChannels:(int)outputchannels;

/// process interleaved float samples from inputBuffer -> libpd -> outputBuffer
/// buffer sizes are based on # of ticks and channels where:
///     size = ticks * [PdBase getBlocksize] * (in/out)channels
/// returns 0 on success
- (int)processFloatWithInputBuffer:(const float *)inputBuffer
                      outputBuffer:(float *)outputBuffer
                             ticks:(int)ticks;

/// process interleaved short samples from inputBuffer -> libpd -> outputBuffer
/// buffer sizes are based on # of ticks and channels where:
///     size = ticks * [PdBase getBlocksize] * (in/out)channels
/// float samples are converted to short by multiplying by 32767 and casting,
/// so any values received from pd patches beyond -1 to 1 will result in garbage
/// note: for efficiency, does *not* clip input
/// returns 0 on success
- (int)processShortWithInputBuffer:(const short *)inputBuffer
                      outputBuffer:(short *)outputBuffer
                             ticks:(int)ticks;

/// process interleaved double samples from inputBuffer -> libpd -> outputBuffer
/// buffer sizes are based on # of ticks and channels where:
///     size = ticks * [PdBase getBlocksize]* (in/out)channels
/// returns 0 on success
- (int)processDoubleWithInputBuffer:(const double *)inputBuffer
					   outputBuffer:(double *)outputBuffer
					          ticks:(int)ticks;

/// enable/disable DSP, aka turn audio processing on/off
- (void)computeAudio:(BOOL)enable;

#pragma mark Array Access

/// get the size of an array by name
/// returns size or negative error code if non-existent
- (int)arraySizeForArrayNamed:(NSString *)arrayName;

/// (re)size an array by name, sizes <= 0 are clipped to 1
/// returns 0 on success or negative error code if non-existent
- (int)resizeArrayNamed:(NSString *)arrayName toSize:(long)size;

/// copy from a named pd array to a float array
/// returns 0 on success or a negative error code if the array is non-existent
/// or offset + n exceeds range of array
- (int)copyArrayNamed:(NSString *)arrayName withOffset:(int)offset
              toArray:(float *)destinationArray count:(int)n;

/// copy from a float array to a named pd array
/// returns 0 on success or a negative error code if the array is non-existent
/// or offset + n exceeds range of array
- (int)copyArray:(float *)sourceArray toArrayNamed:(NSString *)arrayName
	  withOffset:(int)offset count:(int)n;

#pragma mark Sending Messages to Pd

/// send a bang to a destination receiver
/// returns 0 on success or -1 if receiver name is non-existent
/// ex: send a bang to [s foo] on the next tick with:
///     [PdBase sendBangToReceiver:@"foo"];
- (int)sendBangToReceiver:(NSString *)receiverName;

/// send a float to a destination receiver
/// returns 0 on success or -1 if receiver name is non-existent
/// ex: send a 1.0 to [s foo] on the next tick with:
///     [PdBase sendFloat:1 toReceiver:@"foo"];
- (int)sendFloat:(float)value toReceiver:(NSString *)receiverName;

/// send a symbol to a destination receiver
/// returns 0 on success or -1 if receiver name is non-existent
/// ex: send "bar" to [s foo] on the next tick with:
///     [PdBase sendSymbol:@"bar" toReceiver:@"foo"];
- (int)sendSymbol:(NSString *)symbol toReceiver:(NSString *)receiverName;

/// send a list to a destination receiver,
/// the list may be nil to specify an empty list
/// returns 0 on success or -1 if receiver name is non-existent
/// ex: send [list 1 2 bar( to [s foo] on the next tick with:
///     NSArray *list = @[@(1), @(2), @"bar", @"foo"];
///     [PdBase sendList:list toReceiver:@"foo"];
/// note: currently only float and symbol types are supported in pd lists
- (int)sendList:(NSArray *)list toReceiver:(NSString *)receiverName;

/// send a typed message, the list may be nil to specify an empty list

/// send as a typed message to a destination receiver,
/// the list may be nil to specify an empty list
/// note: typed message handling currently only supports up to 4 elements
///       internally, additional elements may be ignored
/// returns 0 on success or -1 if receiver name is non-existent
/// ex: send [; pd dsp 1( on the next tick with:
///     NSArray *list = @[@(1)];
///     [PdBase sendMessage:@"dsp" withArguments:list toReceiver:@"pd"];
/// note: currently only float and symbol types are supported in pd lists
- (int)sendMessage:(NSString *)message withArguments:(NSArray *)list
        toReceiver:(NSString *)receiverName;

#pragma mark Receiving Messages from Pd

/// subscribe to messages sent to a source receiver
/// returns an opaque receiver pointer or nil on failure

/// subscribe to messages sent to a source receiver
/// ex: [PdBase subscribe:@"foo"] adds a "virtual" [r foo] which forwards
/// messages to the PdMessageReceiver delegate
/// returns an opaque receiver pointer or NULL on failure
- (void *)subscribe:(NSString *)symbol;

/// unsubscribe and free a source receiver object created by subscribe
- (void)unsubscribe:(void *)subscription;

/// check if a source receiver object exists with a given name
- (BOOL)exists:(NSString *)symbol;

/// current receiver delegate
@property (nonatomic, weak) id<PdReceiverDelegate> delegate;

/// set receiver delegate and start with polling enabled,
/// setting to nil disconnects the existing delegate and turns off message
/// polling if it's running
///
/// polling is performed by an NSTimer using an interval which is "good enough"
/// for most cases, however if you need lower latency look into calling
/// receiveMessages: using a CADisplayLink of high resolution timer
///
/// for lowest latency, you can disable queing with initializeWithQueue NO
/// although this will result in delegate receiver calls from the audio thread
/// directly which may require manual dispatch to main thread for UI handling
///
/// note: call this from the main thread only
- (void)setDelegate:(id<PdReceiverDelegate>)newDelegate;

/// set the message receiver delegate,
/// setting to nil disconnects the existing delegate and turns off message
/// polling if it's running
/// note: call this from the main thread only
/// set pollingEnabled NO to process messages manually with receiveMessages
- (void)setDelegate:(id<PdReceiverDelegate>)newDelegate
     pollingEnabled:(BOOL)pollingEnabled;

/// process the message queue manually,
/// only required if the respective delegate was set with pollingEnabled NO
/// and queuing is enabled
- (void)receiveMessages;

#pragma mark Sending MIDI Messages to Pd

/// send a MIDI note on message to [notein] objects
/// channel is 0-indexed, pitch is 0-127, and velocity is 0-127
/// channels encode MIDI ports via: libpd_channel = pd_channel + 16 * pd_port
/// note: there is no note off message, send a note on with velocity = 0 instead
/// returns 0 on success or -1 if an argument is out of range
- (int)sendNoteOn:(int)channel pitch:(int)pitch velocity:(int)velocity;

/// send a MIDI control change message to [ctlin] objects
/// channel is 0-indexed, controller is 0-127, and value is 0-127
/// channels encode MIDI ports via: libpd_channel = pd_channel + 16 * pd_port
/// returns 0 on success or -1 if an argument is out of range
- (int)sendControlChange:(int)channel controller:(int)controller
                                           value:(int)value;

/// send a MIDI program change message to [pgmin] objects
/// channel is 0-indexed and value is 0-127
/// channels encode MIDI ports via: libpd_channel = pd_channel + 16 * pd_port
/// returns 0 on success or -1 if an argument is out of range
- (int)sendProgramChange:(int)channel value:(int)value;

/// send a MIDI pitch bend message to [bendin] objects
/// channel is 0-indexed and value is -8192-8192
/// channels encode MIDI ports via: libpd_channel = pd_channel + 16 * pd_port
/// note: [bendin] outputs 0-16383 while [bendout] accepts -8192-8192
/// returns 0 on success or -1 if an argument is out of range
- (int)sendPitchBend:(int)channel value:(int)value;

/// send a MIDI after touch message to [touchin] objects
/// channel is 0-indexed and value is 0-127
/// channels encode MIDI ports via: libpd_channel = pd_channel + 16 * pd_port
/// returns 0 on success or -1 if an argument is out of range
- (int)sendAftertouch:(int)channel value:(int)value;

/// send a MIDI poly after touch message to [polytouchin] objects
/// channel is 0-indexed, pitch is 0-127, and value is 0-127
/// channels encode MIDI ports via: libpd_channel = pd_channel + 16 * pd_port
/// returns 0 on success or -1 if an argument is out of range
- (int)sendPolyAftertouch:(int)channel pitch:(int)pitch
                                       value:(int)value;

/// send a raw MIDI byte to [midiin] objects
/// port is 0-indexed and byte is 0-256
/// returns 0 on success or -1 if an argument is out of range
- (int)sendMidiByte:(int)port byte:(int)byte;

/// send a raw MIDI byte to [sysexin] objects
/// port is 0-indexed and byte is 0-256
/// returns 0 on success or -1 if an argument is out of range
- (int)sendSysex:(int)port byte:(int)byte;

/// send a raw MIDI byte to [realtimein] objects
/// port is 0-indexed and byte is 0-256
/// returns 0 on success or -1 if an argument is out of range
- (int)sendSysRealTime:(int)port byte:(int)byte;

#pragma mark Receiving MIDI Messages from Pd

/// current MIDI receiver delegate
@property (nonatomic, weak) id<PdMidiReceiverDelegate> midiDelegate;

/// set midi receiver and start with polling enabled,
/// setting to nil disconnects the existing delegate and turns off message
/// polling if it's running
///
/// polling is performed by an NSTimer using an interval which is "good enough"
/// for most cases, however if you need lower latency look into calling
/// receiveMidiMessages: using a CADisplayLink of high resolution timer
///
/// for lowest latency, you can disable queing with initializeWithQueue NO
/// although this will result in delegate receiver calls from the audio thread
/// directly which may require manual dispatch to main thread for UI handling
///
/// note: call this from the main thread only
- (void)setMidiDelegate:(id<PdMidiReceiverDelegate>)newDelegate;

/// set the MIDI receiver delegate
/// setting to nil disconnects the existing delegate and turns off message
/// polling if it's running
/// note: call this from the main thread only
/// set pollingEnabled NO to process message queue manually with receiveMidi
- (void)setMidiDelegate:(id<PdMidiReceiverDelegate>)newDelegate
         pollingEnabled:(BOOL)pollingEnabled;

/// Process the MIDI message queue manually,
/// only required if the respective delegate was set with pollingEnabled NO
/// and queuing is enabled
- (void)receiveMidi;

@end
