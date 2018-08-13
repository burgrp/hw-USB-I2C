const driver = require("./driver.js");


async function start() {
	
	let device = await driver.open();

	//while (true)	 {		
		await device.write(0x48, [0]);
		let data = await device.read(0x48, 2);
		console.info(data);
	//}
}

start().catch(console.error);