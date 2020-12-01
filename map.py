import sys

dic = {}
with open('Big5-ZhuYin.map','r', encoding = 'big5-hkscs') as fp:
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


with open('ZhuYin-Big5.map','w', encoding = 'big5-hkscs') as fp:
	for key, value in dic.items():
		output = key + ' ' + ' '.join(value) + '\n'
		fp.write(output)