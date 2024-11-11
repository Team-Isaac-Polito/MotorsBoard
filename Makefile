#List of the environments
ENVIRONMENTS = EncoderTest SmartMotorTest I2C_Test SensorsTest

#Name of the environment we want to test
ENV_TEST = SensorsTest

build:
	pio run -e $(ENV_TEST)
flash: 
	pio run -e $(ENV_TEST) -t upload
clean:
	pio run -t clean