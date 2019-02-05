const usb = require("usb");
const CriticalSection = require("promise-critical-section");

module.exports = {

	open() {
		let device = usb.findByIds(0x0483, 0xFEE0);
		if (!device) {
			throw "USB to I2C converter not found"
		}

		device.open();

		let interface = device.interface(0);
		interface.claim();

		let dataInEp = interface.endpoint(129);
		let dataOutEp = interface.endpoint(1);
		let alertInEp = interface.endpoint(130);

		async function usbRead(len) {
			return new Promise((resolve, reject) => {
				dataInEp.transfer(len, (error, data) => {
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
				dataOutEp.transfer(data, error => {
					if (error) {
						reject(error);
					} else {
						resolve();
					}
				});
			});
		}

		async function usbReadWithCheck(length) {
			let reply = [...(await usbRead(length + 1))];
			let error = reply.shift();
			if (error) {
				throw `I2C error ${error}`;
			}
			return reply;
		}

		const section = new CriticalSection();

		return {
			async read(address, length) {
				try {
					await section.enter();
					await usbWrite(Buffer.from([(address << 1) | 1, length]));
					return await usbReadWithCheck(length);
				} finally {
					await section.leave();
				}
			},

			async write(address, data) {
				try {
					await section.enter();
					await usbWrite(Buffer.from([address << 1].concat(data)));
					await usbReadWithCheck(0);
				} finally {
					await section.leave();
				}
			},

			async alert() {
				return new Promise((resolve, reject) => {
					alertInEp.transfer(1, (error, data) => {
						if (error) {
							reject(error);
						} else {
							resolve(data);
						}
					});
				});
			}

		};

	}

};