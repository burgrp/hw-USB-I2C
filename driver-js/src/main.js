const driver = require("./driver.js");


async function start() {
	
	let device = await driver.open();
	
	let data = await device.read(0x48, 1);
	console.info(data);
	//await device.write(0x7F, [1,2,3,4,5]);
	
}

start().catch(console.error);