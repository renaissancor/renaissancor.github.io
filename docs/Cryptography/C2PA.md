# C2PA (Content Credentials) - Technical Reference

**Last Updated:** February 2026
**Source:** Official C2PA Specifications v2.2+, RFC 3161, RFC 5758

---

## Overview

C2PA (Content Credentials) is a technical standard for verifying the **provenance and authenticity** of digital content through cryptographically verifiable metadata. It is maintained by the [Content Authenticity Initiative](https://c2pa.org/) and represents a unified effort between Adobe's CAI (Content Authenticity Initiative) and Microsoft/BBC's Project Origin.

**Official Specification:** [C2PA Technical Specification v2.2](https://spec.c2pa.org/specifications/specifications/2.2/specs/C2PA_Specification.html) (May 2025)

---

## I. Core Architecture

### 1.1 What is C2PA?

According to the [official specification](https://spec.c2pa.org/), C2PA establishes **provenance through cryptographically verifiable metadata** whose trustworthiness can be assessed based on a defined trust model.

C2PA comprises three interconnected layers:

1. **Assertions** - Labeled data structures documenting asset facts
   - Creation information
   - Edit actions and history
   - Device capture details
   - Thumbnail images
   - Hash bindings to content

2. **Claims** - Digitally signed containers referencing assertions
   - Requires minimum of hard binding assertion and actions assertion
   - Signed with the signer's cryptographic credential

3. **Manifest** - The complete verifiable unit
   - Combines assertions + claims + cryptographic signatures
   - Embedded in assets or stored externally via JUMBF (JPEG Universal Metadata Box Format)

### 1.2 Key Principle: Tamper-Evidence

C2PA is built on strong cryptographic foundations:
- **SHA-256 hashes** for content integrity
- **X.509 certificates** for signer identity
- **Digital signatures** for authenticity proof
- **COSE** (CBOR Object Signing and Encryption) for serialized claim signing

**Any tampering** with the asset or metadata invalidates the cryptographic hash and signature, making alterations immediately detectable.

---

## II. Cryptographic Foundation

### 2.1 Digital Signature Algorithm: ECDSA ES256

C2PA uses **ECDSA with SHA-256 (ES256)** as specified in [RFC 5758](https://datatracker.ietf.org/doc/html/rfc5758):

**RFC 5758 - Internet X.509 Public Key Infrastructure: Additional Algorithms and Identifiers for DSA and ECDSA** (January 2010)

| Component | Specification | Details |
|-----------|---------------|---------|
| **Algorithm** | ECDSA | Elliptic Curve Digital Signature Algorithm [X9.62] |
| **Hash Function** | SHA-256 | 256-bit secure hash (FIPS 180-3) |
| **Combination** | ecdsa-with-SHA256 | OID: iso(1) member-body(2) us(840) ansi-X9-62(10045) signatures(4) ecdsa-with-SHA2(3) 2 |
| **Key Size** | P-256 (256-bit curve) | Also known as "prime256v1" or "secp256r1" |

### 2.2 Public Key Infrastructure

```
┌─────────────────────────────────────────────────────┐
│         Asymmetric Key Pair (ECDSA P-256)          │
├─────────────────────────────────────────────────────┤
│                                                      │
│  Private Key (sk):                                   │
│  • Kept secret by signer                            │
│  • Used to SIGN claims (create signature)           │
│  • 256-bit random value                             │
│  • Stored in HSM or Cloud KMS                       │
│                                                      │
│  Public Key (pk):                                   │
│  • Derived from private key                         │
│  • Embedded in X.509 certificate                    │
│  • Used to VERIFY signatures                        │
│  • Publicly distributed in manifest                 │
│                                                      │
│  Property: verify(message, sign(message, sk), pk) ✓ │
└─────────────────────────────────────────────────────┘
```

**Source:** [RFC 5758 Section 3.2](https://datatracker.ietf.org/doc/html/rfc5758#section-3.2) - ECDSA Signature Algorithm

---

## III. Manifest Verification Process

### 3.1 Signing Process

When creating a C2PA manifest:

1. **Create Claim** - Bundle assertions with signer identity
2. **Serialize** - Convert claim to CBOR format
3. **Hash** - Compute SHA-256 hash of serialized claim
4. **Sign** - Use private key (ECDSA P-256) to create signature
5. **Embed** - Store claim + signature + X.509 certificate in manifest
6. **(Optional) Timestamp** - Send signature hash to RFC 3161 TSA

### 3.2 Verification Process

Validators perform the following checks:

**According to [C2PA Technical Specification](https://spec.c2pa.org/specifications/specifications/2.2/specs/C2PA_Specification.html):**

1. ✓ **Signature Validity** - Verify claim signature using signer's public key
2. ✓ **Hash Integrity** - Confirm each hashed URI in assertions matches actual data
3. ✓ **Certificate Validation** - Check X.509 certificate against trust lists
4. ✓ **Revocation Status** - Verify certificate not revoked (OCSP/CRL)
5. ✓ **Timestamp Validation** (if present) - Confirm RFC 3161 token matches

**Result:** Three possible manifest validation states:
- **Well-formed** - Manifest structure valid
- **Valid** - Signatures verified, structure intact
- **Trusted** - All validation checks pass + signer in trust list

### 3.3 Trust Model Foundation

**From [C2PA Specification v2.2](https://spec.c2pa.org/):**

> "Trust fundamentally relies on the identity of the signer associated with the cryptographic signing key used to sign a claim."

Trust assessment combines:
- Signer identity verification
- X.509 certificate validation
- Credential revocation status
- Time-stamp proof (if applicable)
- Human judgment on combined signals

---

## IV. RFC 3161 Time-Stamp Protocol Integration

### 4.1 The Certificate Expiration Problem

```
WITHOUT RFC 3161:
  2025-01-01: Certificate issued (valid until 2028-01-01)
  2027-06-15: File signed and C2PA manifest created
  2028-02-01: Certificate expires
  2030-01-01: Someone verifies file

  ❌ Result: "Certificate expired - cannot verify"
  ❌ Legal validity lost despite signature created during valid period

WITH RFC 3161:
  2025-01-01: Certificate issued (valid until 2028-01-01)
  2027-06-15: File signed
              TSA proves: "This signature existed on 2027-06-15"
  2028-02-01: Certificate expires
  2030-01-01: Someone verifies file

  ✓ Result: TSA timestamp proves cert was valid at signing time
  ✓ Legal validity maintained indefinitely
```

### 4.2 RFC 3161 Technical Specification

**[RFC 3161 - Internet X.509 Public Key Infrastructure Time-Stamp Protocol (TSP)](https://datatracker.ietf.org/doc/html/rfc3161)** (August 2001)

#### TSA Operational Requirements (RFC 3161 §2.1)

A Time Stamping Authority **MUST**:

1. **Use trustworthy time source** - Atomic clock synchronization
2. **Include accurate time value** - Trustworthy timestamp in each token
3. **Generate unique integers** - Serial number for each token
4. **Hash-only processing** - Never examine actual data content
5. **Sign with dedicated key** - Exclusive timestamping key (`id-kp-timeStamping`)
6. **Support extensions** - Include requested extensions or error

#### TimeStampToken Structure (RFC 3161 §2.4.2)

```
TimeStampReq (Client Request)
├── version: v1
├── messageImprint:
│   ├── hashAlgorithm: "SHA-256"
│   └── hashedMessage: <32 bytes>
├── nonce: <64-bit random> [anti-replay protection]
├── certReq: TRUE [request TSA certificate]
└── extensions: [optional]

                    ↓ HTTPS POST ↓

TimeStampResp (TSA Response)
├── status: "granted" (0)
└── timeStampToken:
    └── TSTInfo:
        ├── version: v1
        ├── policy: TSAPolicyId
        ├── messageImprint: <must match request>
        ├── serialNumber: <unique ID>
        ├── genTime: "20270615123045Z" [UTC]
        ├── accuracy:
        │   ├── seconds: 1
        │   └── millis: 500
        └── nonce: <must match request>
```

**Reference:** [RFC 3161 Section 2.4](https://datatracker.ietf.org/doc/html/rfc3161#section-2.4)

### 4.3 Integration with C2PA

**Workflow:**

```
1. Create & sign C2PA manifest with ES256 private key
2. Hash the signature: sha256(signature) → 32 bytes
3. Create RFC 3161 TimeStampReq with messageImprint
4. POST to TSA endpoint (HTTPS)
5. Receive TimeStampToken
6. Embed token in C2PA manifest
7. Result: Manifest valid even after certificate expiry
```

**Cost Model:** Each signature requires ONE TSA API call
- Per-request pricing: $0.01-0.15 per timestamp (volume-based discounts)
- Unlimited subscription: $500-2,000/month for high-volume

---

## V. Manifest Limitations & Considerations

### 5.1 Manifest Loss on Capture/Upload

**Critical Limitation:** The C2PA manifest is embedded in file metadata and is **lost** during:
- Screenshots or screen captures
- Social media uploads (Instagram, Twitter, TikTok)
- Messaging app compression (WhatsApp, Telegram, Slack)
- Cloud storage optimization

This occurs because these platforms strip metadata for:
- Storage optimization
- Privacy protection
- Bandwidth reduction

**Result:** A legitimately signed file loses all C2PA authentication when shared.

### 5.2 Addressing Manifest Loss

**Potential Solutions Under Research:**

1. **Steganography (Steg AI)**
   - Embed authenticity information visually in the image
   - Hide metadata as imperceptible patterns
   - Survives screenshots and compression

2. **Blockchain Anchoring**
   - Record manifest hash on public blockchain
   - Enables recovery even if manifest is lost
   - Verifiable through immutable ledger

3. **Multi-Platform Distribution**
   - Publish manifest separately (e.g., IPFS, proprietary server)
   - Reference via file hash or URL
   - Requires user awareness to verify

---

## VI. Security Considerations

### 6.1 Private Key Management

**Critical Risk:** If private key is compromised, attackers can create false C2PA credentials.

**Mitigation:**

| Strategy | Implementation | Security Level |
|----------|-----------------|-----------------|
| **HSM (Hardware Security Module)** | Thales Luna, YubiHSM | ★★★★★ Highest |
| **Cloud KMS** | AWS KMS, Azure Key Vault | ★★★★☆ High |
| **Local Encryption** | Encrypted .key file + password | ★★☆☆☆ Low |
| **Environment Variable** | .env file | ★☆☆☆☆ Dangerous |

**Recommendation:** Use HSM for production environments.

**Reference:** [RFC 3161 §4 - Security Considerations](https://datatracker.ietf.org/doc/html/rfc3161#section-4)

### 6.2 TSA Provider Selection

**Key Requirements:**

- ✓ RFC 3161 Compliant
- ✓ Reputable CA (DigiCert, GlobalSign, Sectigo)
- ✓ SLA guarantees (99.9% uptime)
- ✓ Legal responsibility for timestamp authenticity
- ✓ Long-term record retention (≥10 years)
- ✓ HTTPS with valid certificate

**Recommended Providers:**

| Provider | Compliance | Notes |
|----------|-----------|-------|
| **DigiCert** | RFC 3161 ✓ | Global leader, SLA 99.9%, legal backing |
| **GlobalSign** | RFC 3161 ✓ | EU eIDAS certified, international support |
| **Sectigo** | RFC 3161 ✓ | SMB-friendly pricing, fast API |
| **FreeTSA** | RFC 3161 ✓ | Free (testing only), no SLA, limited trust |

### 6.3 Certificate Lifecycle Management

**Renewal Schedule:**
- Certificates expire in **1-3 years**
- Begin renewal **90 days before** expiration
- Maintain parallel operation during transition (2-4 weeks)
- Retain old certificates for verification purposes

**Monitoring:**

```python
# Example: Certificate expiry alert
from datetime import datetime, timedelta
from cryptography import x509

def check_certificate_expiry(cert_path: str, warning_days: int = 90):
    with open(cert_path, 'rb') as f:
        cert = x509.load_pem_x509_certificate(f.read())

    days_remaining = (cert.not_valid_after - datetime.utcnow()).days

    if days_remaining < 0:
        return {"status": "EXPIRED", "message": "Certificate expired!"}
    elif days_remaining < warning_days:
        return {"status": "WARNING", "message": f"Expires in {days_remaining} days"}
    else:
        return {"status": "OK", "message": f"Valid {days_remaining} days"}
```

---

## VII. Implementation Architecture

### 7.1 System Flow

```
┌─────────────────────────────────────────────────────────┐
│                   Backend Server (FastAPI)              │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  POST /sign-image                                      │
│  1. Receive file + metadata                            │
│  2. Create C2PA assertion (metadata, hashes)           │
│  3. Create C2PA claim (assertion + signer ID)          │
│  4. Sign claim with ECDSA P-256 private key            │
│  5. Hash signature: SHA-256(signature)                 │
│     │                                                   │
│     └─→ [RFC 3161 TSA Client]                          │
│         ├─ Create TimeStampReq (messageImprint)        │
│         ├─ HTTPS POST to TSA_URL                       │
│         ├─ Verify TimeStampResp                        │
│         └─ Extract timestamp token                     │
│  6. Embed token in manifest                            │
│  7. Package manifest into file                         │
│  8. Return signed file                                 │
│                                                         │
└─────────────────────────────────────────────────────────┘
              ↓
        ┌──────────────────┐
        │  TSA Provider    │
        │  (DigiCert)      │
        ├──────────────────┤
        │ RFC 3161 Endpoint│
        │ • Atomic clock   │
        │ • Signature gen  │
        │ • Token creation │
        └──────────────────┘
```

### 7.2 Recommended Technology Stack

| Component | Technology | Reference |
|-----------|-----------|-----------|
| **Signing Algorithm** | ECDSA P-256 (ES256) | [RFC 5758](https://datatracker.ietf.org/doc/html/rfc5758) |
| **Hash Algorithm** | SHA-256 | FIPS 180-3 |
| **TSA Protocol** | RFC 3161 (HTTP) | [RFC 3161](https://datatracker.ietf.org/doc/html/rfc3161#section-3.4) |
| **ASN.1 Encoding** | DER (Distinguished Encoding Rules) | X.690 standard |
| **Python Library** | asn1crypto | [GitHub: wbond/asn1crypto](https://github.com/wbond/asn1crypto) |
| **Key Storage** | HSM or Cloud KMS | AWS KMS / Azure Key Vault |

---

## VIII. Validation Checklist

### Pre-Deployment Security

- [ ] Private key stored in HSM or KMS (never in .env)
- [ ] TSA endpoint HTTPS with valid CA certificate
- [ ] RFC 3161 ASN.1 encoding/decoding implemented
- [ ] Nonce generation for replay attack prevention
- [ ] Certificate expiration monitoring (90-day alert)
- [ ] TSA provider SLA review and legal agreements
- [ ] Fallback logic if TSA unavailable
- [ ] Timestamp verification in manifest validation
- [ ] Access logs for key usage auditing
- [ ] Incident response plan for key compromise

---

## IX. References

### Official Specifications

1. [**C2PA Technical Specification v2.2** (May 2025)](https://spec.c2pa.org/specifications/specifications/2.2/specs/C2PA_Specification.html)
   - Official C2PA architecture and manifest format
   - Assertion types and claim structure
   - Validation requirements

2. [**RFC 3161** - Internet X.509 Public Key Infrastructure Time-Stamp Protocol (TSP)](https://datatracker.ietf.org/doc/html/rfc3161)
   - TimeStampReq/TimeStampResp format
   - TSA operational requirements
   - MIME types and HTTP transport
   - Security considerations

3. [**RFC 5758** - Internet X.509 PKI: Additional Algorithms for DSA and ECDSA](https://datatracker.ietf.org/doc/html/rfc5758)
   - ECDSA with SHA-256 (ES256) specification
   - Algorithm identifiers and ASN.1 encoding
   - X.509 certificate requirements

4. [**FIPS 180-3** - Secure Hash Standard (SHA)](https://csrc.nist.gov/publications/fips)
   - SHA-256 specification and properties

5. [**X9.62** - ANSI Standard for ECDSA](https://webstore.ansi.org/)
   - Elliptic Curve Digital Signature Algorithm

### Implementation Resources

- [**C2PA Viewer**](https://c2paviewer.com/) - Verify C2PA credentials in media
- [**C2PA Studio**](https://c2pa.studio/) - Manage content provenance
- [**OpenSSL ts** command](https://www.openssl.org/docs/man3.0/man1/openssl-ts.html) - RFC 3161 client
- [**asn1crypto Python Library**](https://github.com/wbond/asn1crypto) - ASN.1 DER encoding
- [**tsp-client Python Package**](https://github.com/pyauth/tsp-client) - RFC 3161 TSP client

### TSA Providers

- [**DigiCert Time-Stamp Authority**](https://www.digicert.com/timestamp-server)
- [**GlobalSign Time-Stamp Service**](https://www.globalsign.com/en/timestamp-service)
- [**Sectigo (Comodo) TSA**](https://www.sectigo.com/)

### Additional Reading

- [**Content Authenticity Initiative**](https://contentauthenticity.org/) - C2PA adoption and resources
- [**Trusted Timestamping (Wikipedia)**](https://en.wikipedia.org/wiki/Trusted_timestamping) - General background
- [**RFC 3161 Digital Forensics**](https://www.metaspike.com/trusted-timestamping-rfc-3161-digital-forensics/) - Forensic applications

---

**Document Version:** 1.0
**Last Updated:** February 11, 2026
**Status:** Production Reference