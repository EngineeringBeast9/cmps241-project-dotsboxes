# Security Policy

## Supported Versions

Only the latest commit on `main` is supported.

## Reporting a Vulnerability

Do not open a public issue for security problems. Contact the maintainers directly (Malek or Ali) with:

- A description of the issue and its impact
- Steps to reproduce
- Any suggested fix

We will acknowledge reports and work on a fix before any public disclosure.

## Secure Development Practices

- Input is validated for range, adjacency, duplicates, and non-numeric values before any board change.
- Builds are checked with `-Wall -Wextra`, and memory is checked with Valgrind (zero leaks, zero errors).
- Dependabot vulnerability alerts are enabled for this repository.
