# Sample Prog.

set A = {1, 2, 3}			# Initialize a set.

declare int i				# Primitves don't have to be explicitly initialized (default 0 for int).

while i < |A|				# "While i < cardinality of set A" (Note: The opening brace 'must' be on a new line).
{					 
	if (i < 2)			# Parentheses are optional. This would work without them also 
	{				# Here too, the brace HAS to open on a new line.
		print A[i]		# Print the i'th element of A.
		print '\\n'		# Print character (two delimiting '\' needed). Literals to be printed are immediately destroyed.
	}				# If preceded by an expression (not a command, like '--quit--' or 'declare'), the closing brace must also be on a new line. 
	else { } 			# The else opening braces are versatile and are okay anywhere, since we don't have a conditional expression to parse.
					# The closing brace can also be kept on the same line since it is not preceded by an expression.
	let i = i + 1			# This is an assignment. Still haven't been able to get assigments of the form `let a[b] = c` to work. Hmm.
}

delete i				# Memory is 'not' managed automatically, for now.

set B = . A				# Make a 'deep_copy' of A

delete A				# ... for now. (Containers recursively delete their data).

print B					# So if B still prints, deep_copy successful!

print B U {{'H', 'S'}}			# Sets do not have to be homogeneous.

delete B				# Delete the deep_copy.

quit					# That's all for now, folks!