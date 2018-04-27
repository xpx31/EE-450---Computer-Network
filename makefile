all:
	gcc -o client client.c
	gcc -o serverA serverA.c
	gcc -o serverB serverB.c
	gcc -o serverC serverC.c
	gcc -o aws aws.c

.PHONY: client
client:
	./client

.PHONY: serverA
serverA:
	./serverA

.PHONY: serverB
serverB:
	./serverB

.PHONY: serverC
serverC:
	./serverC

.PHONY: aws
aws:
	./aws