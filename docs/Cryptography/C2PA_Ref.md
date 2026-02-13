# C2PA TSP 

**Certificate Expiration vs. Long-term Trust**: Your C2PA signing certificates have finite lifetimes (typically 1-3 years). Without TSP, once a certificate expires, you cannot prove that files signed *before expiration* were authentic. TSP provides **long-term validity** even after certificate expiration.

## How TSP Works in Your Context

### 1. **Third-Party Time Proof**
When you sign a file with C2PA:
- Your system sends the file's hash to a trusted TSA (Timestamping Authority)
- TSA returns a cryptographically signed timestamp proving "this hash existed at time X"
- This timestamp gets embedded in the C2PA manifest
- The TSA's signature is separate from your company's signing certificate

### 2. **Preservation of Trust After Certificate Expiration**

```
Without TSP:
Certificate expires 2028-01-01
File signed 2027-06-15 → Cannot verify after 2028-01-01 

With TSP:
Certificate expires 2028-01-01  
File signed 2027-06-15 with TSA timestamp
→ TSA proves: "This signature existed on 2027-06-15"
→ Can verify indefinitely because timestamp proves signing happened during valid period ✓
```

### 3. **Key Security Features from RFC 3161**

**Requirements the TSA Must Meet**:
- Use trustworthy time source (atomic clock synchronization)
- Generate unique serial numbers for each timestamp
- Only timestamp hash values (never sees original content)
- Sign with dedicated timestamping key (`id-kp-timeStamping`)
- Include accuracy information (±seconds/milliseconds)

**What Gets Embedded in Your C2PA Manifest**:
```typescript
TSTInfo {
  version: v1(1),
  policy: TSAPolicyId,           // TSA's policy identifier
  messageImprint: {
    hashAlgorithm: "SHA-256",
    hashedMessage: "abc123..."   // Hash of your C2PA signature
  },
  serialNumber: 1234567,         // Unique timestamp token ID
  genTime: "20270615123045Z",    // Precise timestamp
  accuracy: {                    // Time accuracy bounds
    seconds: 1,
    millis: 500
  },
  tsa: GeneralName              // TSA identity
}
```

## Integration with Your Project

### Current Implementation Gap
Your `backend/app/signing/c2pa_signer.py` likely generates C2PA manifests but may not be calling a TSA. To add RFC 3161 support:

```python
# Pseudocode for TSP integration
def sign_with_timestamp(manifest_data):
    # 1. Sign the manifest with your ES256 key
    signature = es256_sign(manifest_data)
    
    # 2. Hash the signature
    signature_hash = sha256(signature)
    
    # 3. Create TimeStampReq
    tsp_request = {
        "version": 1,
        "messageImprint": {
            "hashAlgorithm": "SHA-256",
            "hashedMessage": signature_hash
        },
        "certReq": True,  # Request TSA certificate
        "nonce": generate_random_nonce()
    }
    
    # 4. Send to TSA (DigiCert, GlobalSign, etc.)
    tsp_response = requests.post(
        TSA_URL,
        headers={"Content-Type": "application/timestamp-query"},
        data=encode_asn1(tsp_request)
    )
    
    # 5. Verify TSP response
    timestamp_token = verify_tsp_response(tsp_response)
    
    # 6. Embed in C2PA manifest
    manifest_data["timestamp"] = timestamp_token
    return manifest_data
```

### Cost Structure Implications

From your notes, you need to account for:
- **Per-request TSA API costs**: Every file you sign requires one TSP call
- **Batch processing impact**: With 20 files in a ZIP, that's 20 TSA API calls
- **Recommended providers**: DigiCert, GlobalSign (mentioned in RFC and your docs)
- **Multiple TSA strategy**: Using 2+ TSAs for redundancy (security best practice)

## Critical Security Benefits

### 1. **Compromise Detection**
If your private key is compromised and you revoke the certificate:
- Files with TSA timestamps *before* revocation date remain valid
- Files without timestamps become unverifiable
- Audit trail helps distinguish genuine vs. fraudulent signatures

### 2. **Replay Attack Prevention**
The `nonce` field in TSP (large random number) prevents:
- Network middleman delays/replays
- Verification that response matches specific request
- Detection of duplicate timestamps on same content

### 3. **Compliance Alignment**
- Meets **non-repudiation** requirements for legal evidence
- Satisfies EU eIDAS regulations for qualified timestamps
- Aligns with ISO/IEC 10181-5 security frameworks

## Recommended Next Steps

1. **Choose TSA Provider**: DigiCert or GlobalSign (RFC-recommended, widely trusted)
2. **Implement TSP Client**: Add RFC 3161 ASN.1 encoding/decoding to `c2pa_signer.py`
3. **Update API Costs**: Factor TSA per-call pricing into `/process` and `/process-batch` endpoints
4. **Test Certificate Expiration**: Verify files remain valid after cert expiry with TSP
5. **Add Timestamp Verification**: Update verification logic to check `TSTInfo` fields

The TSP is not optional for production C2PA—it's the **foundation of long-term authenticity** and directly addresses the "certificate expiration problem" highlighted in your notes.