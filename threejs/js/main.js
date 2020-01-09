const canvas = document.getElementById("canvas");
const sceneManager = new SceneManager(canvas);

sceneManager.loadFishMesh("../obj/fish/Fish1",render);

bindEventListeners();
//render();

function bindEventListeners() {
	//window.onresize = resizeCanvas;
	//resizeCanvas();
	//window.addEventListener( 'wheel', scrollEvent, false );
	window.addEventListener( 'resize', resizeCanvas, false );
	resizeCanvas();
}

function resizeCanvas() {
	canvas.style.width = '100%';
	canvas.style.height= '100%';

	canvas.width  = canvas.offsetWidth;
	canvas.height = canvas.offsetHeight;

	sceneManager.onWindowResize();
}

function scrollEvent() {
	event.preventDefault();
	sceneManager.onDocumentMouseWheel(event.deltaY*0.005);
}

function render() {
	requestAnimationFrame(render);
	sceneManager.update();
}
