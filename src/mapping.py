import sys
if len(sys.argv) < 2:
	print("No argv1, use default: Big5-ZhuYin.map")
	argv1 = 'Big5-ZhuYin.map'
else:
	argv1 = sys.argv[1]
if len(sys.argv) < 3:
	print("No argv2, use default: ZhuYin-Big5.map")
	argv2 = 'ZhuYin-Big5.map'
else:
	argv2 = sys.argv[2]

dic = {}
with open(argv1,'r', encoding = 'big5-hkscs') as fp:
	all_lines = fp.readlines()
	for line in all_lines:
		first, secs = line.split()
		secs = secs.split('/')
		if first not in dic:
			dic[first] = [first]
		for sec in secs:
			if sec[0] not in dic:
				dic[sec[0]] = [first]
			elif first not in dic[sec[0]]:
				dic[sec[0]].append(first)


with open(argv2,'w', encoding = 'big5-hkscs') as fp:
	for key, value in dic.items():
		output = key + ' ' + ' '.join(value) + ' \n'
		fp.write(output)