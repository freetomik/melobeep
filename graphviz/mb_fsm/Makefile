name=mb_fsm
suf=svg
viewer=inkview

all:
	dot -T$(suf) $(name).dot > $(name).$(suf)
	$(viewer) $(name).$(suf) &

view:
	$(viewer) $(name).$(suf) &
