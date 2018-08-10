const usb = require("usb");

module.exports = {

	open() {
		let device = usb.findByIds(0x0483, 0xFEE0);
		if (!device) {
			throw "USB to I2C converter not found"
		}
		
		device.open();

		let interface = device.interface(0);
		interface.claim();

		let inEndpoint = interface.endpoint(129);
		let outEndpoint = interface.endpoint(1);

		async function read(len) {
			return new Promise((resolve, reject) => {
				inEndpoint.transfer(len, (error, data) => {
					if (error) {
						reject(error);
					} else {
						resolve(data);
					}
				});
			});
		}

		async function write(data) {
			return new Promise((resolve, reject) => {
				outEndpoint.transfer(data, error => {
					if (error) {
						reject(error);
					} else {
						resolve();
					}
				});
			});
		}


		return {

			async read(address, length) {
				await write(Buffer.from([(address << 1) | 1, length]));
				return [...(await read(length))];
			},

			async write(address, data) {
				await write(Buffer.from([address << 1].concat(data)));
			}

		};

	}

};