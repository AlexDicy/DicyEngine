<script lang="ts">
	const register = window.register ?? ((_: Function) => {
	});

	let deltaTime = $state(0);

	register((dT: number) => {
		console.log('deltaTime', deltaTime);
		deltaTime = dT;
	});
</script>

<h1>Welcome to SvelteKit</h1>
<p>Visit <a href="https://svelte.dev/docs/kit">svelte.dev/docs/kit</a> to read the documentation</p>
<pre>deltaTime: {deltaTime}</pre>
