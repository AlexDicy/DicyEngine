<script lang="ts">
	let deltaTime = $state(0);
	const fps = $derived(deltaTime ? (1.0 / deltaTime).toFixed(2) : 0);

	window.setMessageListener('updateFrameInfo', (dT: number) => {
		console.log('deltaTime', deltaTime);
		deltaTime = dT;
	});

</script>

<pre>deltaTime: {deltaTime}</pre>
<pre>FPS: {fps}</pre>
