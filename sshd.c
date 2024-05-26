// authentication -> ssh-ed25519
// key exchange -> curve25519-sha256
// cipher -> chacha20-poly1305@openssh.com
// mac -> none

//<-
//SSH-2.0-omiltem<CR><LF>

//->
//SSH-2.0-VERSION<CR><LF>

//->
//byte         SSH_MSG_KEXINIT
//byte[16]     cookie (random bytes)
//name-list    kex_algorithms
//name-list    server_host_key_algorithms
//name-list    encryption_algorithms_client_to_server
//name-list    encryption_algorithms_server_to_client
//name-list    mac_algorithms_client_to_server
//name-list    mac_algorithms_server_to_client
//name-list    compression_algorithms_client_to_server
//name-list    compression_algorithms_server_to_client
//name-list    languages_client_to_server
//name-list    languages_server_to_client
//boolean      first_kex_packet_follows
//uint32       0 (reserved for future extension)

//<-
//byte         SSH_MSG_KEXINIT
//byte[16]     cookie (random bytes)
//name-list    kex_algorithms
//name-list    server_host_key_algorithms
//name-list    encryption_algorithms_client_to_server
//name-list    encryption_algorithms_server_to_client
//name-list    mac_algorithms_client_to_server
//name-list    mac_algorithms_server_to_client
//name-list    compression_algorithms_client_to_server
//name-list    compression_algorithms_server_to_client
//name-list    languages_client_to_server
//name-list    languages_server_to_client
//boolean      first_kex_packet_follows
//uint32       0 (reserved for future extension)

//->
//byte     SSH_MSG_KEX_ECDH_INIT
//string   Q_C, client's ephemeral public key octet string

//<-
//byte     SSH_MSG_KEX_ECDH_REPLY
//string   K_S, server's public host key
//string   Q_S, server's ephemeral public key octet string
//string   the signature on the exchange hash

//signature of
//string   V_C, client's identification string (CR and LF excluded)
//string   V_S, server's identification string (CR and LF excluded)
//string   I_C, payload of the client's SSH_MSG_KEXINIT
//string   I_S, payload of the server's SSH_MSG_KEXINIT
//string   K_S, server's public host key
//string   Q_C, client's ephemeral public key octet string
//string   Q_S, server's ephemeral public key octet string
//mpint    K,   shared secret

//->
//byte      SSH_MSG_USERAUTH_REQUEST
//string    user name in ISO-10646 UTF-8 encoding [RFC3629]
//string    service name in US-ASCII
//string    "publickey"
//boolean   FALSE
//string    public key algorithm name
//string    public key blob

//<-
//byte      SSH_MSG_USERAUTH_PK_OK
//string    public key algorithm name from the request
//string    public key blob from the request

//->
//byte      SSH_MSG_USERAUTH_REQUEST
//string    user name
//string    service name
//string    "publickey"
//boolean   TRUE
//string    public key algorithm name
//string    public key to be used for authentication
//string    signature

//signature of
//string    session identifier
//byte      SSH_MSG_USERAUTH_REQUEST
//string    user name
//string    service name
//string    "publickey"
//boolean   TRUE
//string    public key algorithm name
//string    public key to be used for authentication

//<-
//SSH_MSG_USERAUTH_PK_OK


_rdrand(): int;
main(argc: int, argv: **byte, envp: **byte) {
	exit(_rdrand());
}
