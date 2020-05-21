all:
	@cd src ; make
	@cd src ; mv ./scheduler ../

clean:
	-rm -f ./scheduler
	@cd src ; make clean