const driver = require("./driver.js");


async function start() {
	
	let device = await driver.open();

	console.info("Scanning I2C bus...")

	for (let address = 0x00; address < 0x78; address++) {
		try {
			await device.write(address, []);
			console.info("0x" + ("0" + address.toString(16)).slice(-2));
		} catch(e) {			
		}
	}

	console.info("That's all.")
	
}

start().catch(console.error);