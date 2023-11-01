function playSong() {
	let sampleRate = 44100;
	let sampleByteSize = 2;
	let channelCount = 2;
	let frameCount = sampleRate * 60;
	let sampleCount = channelCount * frameCount;
	
	let tmpBuffer = new Uint8ClampedArray(sampleByteSize * sampleCount);
	
	console.log('Rendering song lol');
	Module.ccall('renderSong', null, ['array', 'number'], [tmpBuffer, sampleCount]);
	console.log('Finished rendering song');

	let audioContext = new AudioContext({ sampleRate });
	
	let tmpBufferView = new DataView(tmpBuffer);
	let audioBuffer = audioContext.createBuffer(channelCount, frameCount, sampleRate);
	
	let channelBuffers = [
		audioBuffer.getChannelData(0),
		audioBuffer.getChannelData(1),
	];
	
	for (let sampleIndex = 0; sampleIndex < frameCount; ++sampleIndex) {
		let byteOffset = sampleIndex * sampleByteSize;
		let sample = tmpBufferView.getInt16(byteOffset, true) / 0x7fff;
		channelBuffers[sampleIndex % channelCount][sampleIndex / channelCount] = sample;
	}
	
	let audioBufferSourceNode = audioContext.createBufferSource();
	audioBufferSourceNode.buffer = audioBuffer;
	audioBufferSourceNode.connect(audioContext.destination);
	audioBufferSourceNode.start();
}
