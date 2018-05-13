#based on https://0day.work/how-i-recovered-your-private-key-or-why-small-keys-are-bad/
#!/usr/bin/python2
import pyasn1.codec.der.encoder
import pyasn1.type.univ
import base64

def recover_key(p, q, e, output_file):

	def egcd(a, b):
	    x,y, u,v = 0,1, 1,0
	    while a != 0:
	        q, r = b//a, b%a
	        m, n = x-u*q, y-v*q
	        b,a, x,y, u,v = a,r, u,v, m,n
	    gcd = b
	    return gcd, x, y

	def modinv(a, m):
	    gcd, x, y = egcd(a, m)
	    if gcd != 1:
	        return None  # modular inverse does not exist
	    else:
	        return x % m

	def pempriv(n, e, d, p, q, dP, dQ, qInv):
	    template = '-----BEGIN RSA PRIVATE KEY-----\n{}-----END RSA PRIVATE KEY-----\n'
	    seq = pyasn1.type.univ.Sequence()
	    for x in [0, n, e, d, p, q, dP, dQ, qInv]:
	        seq.setComponentByPosition(len(seq), pyasn1.type.univ.Integer(x))
	    der = pyasn1.codec.der.encoder.encode(seq)
	    return template.format(base64.encodestring(der).decode('ascii'))

	n = p * q
	phi = (p -1)*(q-1)
	d = modinv(e, phi)
	dp = modinv(e,(p-1))
	dq = modinv(e,(q-1))
	qi = modinv(q,p)

	key = pempriv(n, e, d, p, q, dp, dq, qi)

	f = open(output_file,"w")
	f.write(key)
	f.close()


recover_key(1385409854850246784644682622624349784560468558795524903, 1524938362073628791222322453937223798227099080053904149, 65537, "test.pem")
