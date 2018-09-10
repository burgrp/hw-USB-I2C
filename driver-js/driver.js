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

		async function usbRead(len) {
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

		async function usbWrite(data) {
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

		async function readAndCheckError(length) {
			let reply = [...(await usbRead(length + 1))];
			let error = reply.shift();
			if (error) {
				throw `I2C error ${error}`;
			}
			return reply;
		}

		async function doRead(address, length) {
			await usbWrite(Buffer.from([(address << 1) | 1, length]));
			return await readAndCheckError(length);
		}

		async function doWrite(address, data) {
			await usbWrite(Buffer.from([address << 1].concat(data)));
			await readAndCheckError(0);
		}

		let queue = [];

		function checkQueue() {
			if (queue.length === 1) {
				queue[0]();
			}
		}

		return {
			
			read(address, length) {
				return new Promise((resolve, reject) => {
					queue.push(() => {
						doRead(address, length).then(r => {
							queue.shift();
							resolve(r);
							checkQueue();							
						}, e => {
							queue.shift();
							reject(e);
							checkQueue();							
						});
					});
					checkQueue();
				});
			},

			write(address, data) {
				return new Promise((resolve, reject) => {
					queue.push(() => {
						doWrite(address, data).then(r => {
							queue.shift();
							resolve(r);
							checkQueue();							
						}, e => {
							queue.shift();
							reject(e);
							checkQueue();							
						});
					});
					checkQueue();
				});
			}

		};

	}

};