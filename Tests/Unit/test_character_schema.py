"""
W.H.A.T. I.F. — Character Schema Unit Tests
============================================
Phase 0 validation: Ensures the character JSON schema is well-formed and
that character data instances correctly validate against it.

Usage:
    pip install jsonschema
    python test_character_schema.py

Or with pytest:
    pip install pytest jsonschema
    pytest test_character_schema.py -v
"""

import json
import pathlib
import sys

try:
    import jsonschema
    from jsonschema import validate, ValidationError, SchemaError
except ImportError:
    print("ERROR: jsonschema not installed. Run: pip install jsonschema")
    sys.exit(1)


# ---------------------------------------------------------------------------
# Paths
# ---------------------------------------------------------------------------
TESTS_DIR    = pathlib.Path(__file__).parent
PROJECT_ROOT = TESTS_DIR.parent.parent
SCHEMA_PATH  = PROJECT_ROOT / "Docs" / "Schemas" / "character_schema.json"
FIXTURES_DIR = TESTS_DIR / "fixtures"


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------
def load_json(path: pathlib.Path) -> dict:
    if not path.exists():
        raise FileNotFoundError(f"Expected file not found: {path}")
    return json.loads(path.read_text(encoding="utf-8"))


def assert_valid(instance: dict, schema: dict, label: str) -> None:
    validate(instance=instance, schema=schema)
    print(f"  ✅ PASS  {label}")


def assert_invalid(instance: dict, schema: dict, label: str) -> None:
    try:
        validate(instance=instance, schema=schema)
        print(f"  ❌ FAIL  {label}  — expected ValidationError, got none")
        sys.exit(1)
    except ValidationError:
        print(f"  ✅ PASS  {label}  (correctly rejected)")


# ---------------------------------------------------------------------------
# Tests
# ---------------------------------------------------------------------------
def test_schema_is_valid_json_schema(schema: dict) -> None:
    """The schema file must itself be a valid JSON Schema Draft-07 document."""
    meta_schema_url = "http://json-schema.org/draft-07/schema#"
    try:
        jsonschema.Draft7Validator.check_schema(schema)
        print(f"  ✅ PASS  Schema is a valid JSON Schema Draft-07 document")
    except SchemaError as e:
        print(f"  ❌ FAIL  Schema is not valid: {e.message}")
        sys.exit(1)


def test_valid_sample_character(schema: dict) -> None:
    """The fixture file must validate successfully against the schema."""
    fixture_path = FIXTURES_DIR / "sample_character_base.json"
    instance = load_json(fixture_path)
    assert_valid(instance, schema, f"sample_character_base.json validates against schema")


def test_missing_character_id(schema: dict) -> None:
    """A character missing 'character_id' must be rejected."""
    bad = {
        "display_name": "Nameless",
        "description": "No ID",
        "default_faction": "Neutral",
        "character_tags": [],
        "base_attribute_values": {},
        "personality": {
            "aggression": 0.5, "loyalty": 0.5, "recklessness": 0.5,
            "creativity": 0.5, "self_preservation": 0.5, "altruism": 0.5
        },
        "default_combat_stance": "Balanced"
    }
    assert_invalid(bad, schema, "Missing 'character_id' is rejected")


def test_invalid_faction_value(schema: dict) -> None:
    """A character with an unrecognized faction must be rejected."""
    bad = {
        "character_id": "test_char",
        "display_name": "Test",
        "description": "Test description",
        "default_faction": "INVALID_FACTION",   # Not in enum
        "character_tags": [],
        "base_attribute_values": {},
        "personality": {
            "aggression": 0.5, "loyalty": 0.5, "recklessness": 0.5,
            "creativity": 0.5, "self_preservation": 0.5, "altruism": 0.5
        },
        "default_combat_stance": "Balanced"
    }
    assert_invalid(bad, schema, "Invalid 'default_faction' enum value is rejected")


def test_personality_out_of_range(schema: dict) -> None:
    """Personality values outside [0.0, 1.0] must be rejected."""
    bad = {
        "character_id": "test_char_b",
        "display_name": "Test B",
        "description": "Test description",
        "default_faction": "Hero",
        "character_tags": [],
        "base_attribute_values": {},
        "personality": {
            "aggression": 1.5,   # > 1.0 — invalid
            "loyalty": 0.5, "recklessness": 0.5,
            "creativity": 0.5, "self_preservation": 0.5, "altruism": 0.5
        },
        "default_combat_stance": "Balanced"
    }
    assert_invalid(bad, schema, "Personality value > 1.0 is rejected")


def test_invalid_character_id_format(schema: dict) -> None:
    """character_id must be lowercase snake_case only."""
    bad = {
        "character_id": "My Character ID!",   # spaces and ! not allowed
        "display_name": "Test C",
        "description": "Test description",
        "default_faction": "Hero",
        "character_tags": [],
        "base_attribute_values": {},
        "personality": {
            "aggression": 0.5, "loyalty": 0.5, "recklessness": 0.5,
            "creativity": 0.5, "self_preservation": 0.5, "altruism": 0.5
        },
        "default_combat_stance": "Balanced"
    }
    assert_invalid(bad, schema, "character_id with spaces/special chars is rejected")


def test_relationship_affinity_out_of_range(schema: dict) -> None:
    """Relationship affinity must be between -1.0 and 1.0."""
    bad = {
        "character_id": "test_char_c",
        "display_name": "Test D",
        "description": "Test description",
        "default_faction": "Hero",
        "character_tags": [],
        "base_attribute_values": {},
        "personality": {
            "aggression": 0.5, "loyalty": 0.5, "recklessness": 0.5,
            "creativity": 0.5, "self_preservation": 0.5, "altruism": 0.5
        },
        "default_combat_stance": "Balanced",
        "base_relationships": [
            {
                "target_character_id": "some_enemy",
                "affinity_value": -2.0,     # < -1.0 — invalid
                "relationship_type": "Enemy"
            }
        ]
    }
    assert_invalid(bad, schema, "Relationship affinity < -1.0 is rejected")


def test_minimal_valid_character(schema: dict) -> None:
    """A character with only required fields must validate successfully."""
    minimal = {
        "character_id": "minimal_test_char",
        "display_name": "Minimal Character",
        "description": "A minimal valid character for testing.",
        "default_faction": "Neutral",
        "character_tags": ["Character.Role.Scout"],
        "base_attribute_values": {
            "Attribute.Strength": 100.0,
            "Attribute.Durability": 100.0
        },
        "personality": {
            "aggression": 0.5,
            "loyalty": 0.5,
            "recklessness": 0.3,
            "creativity": 0.6,
            "self_preservation": 0.7,
            "altruism": 0.4
        },
        "default_combat_stance": "Balanced"
    }
    assert_valid(minimal, schema, "Minimal valid character (required fields only) validates")


def test_attribute_value_out_of_range(schema: dict) -> None:
    """Known attribute values above 1000 must be rejected."""
    bad = {
        "character_id": "overpowered_char",
        "display_name": "Overpowered",
        "description": "Test description",
        "default_faction": "Cosmic",
        "character_tags": [],
        "base_attribute_values": {
            "Attribute.Strength": 9999.0   # > 1000 max
        },
        "personality": {
            "aggression": 0.5, "loyalty": 0.5, "recklessness": 0.5,
            "creativity": 0.5, "self_preservation": 0.5, "altruism": 0.5
        },
        "default_combat_stance": "Aggressive"
    }
    assert_invalid(bad, schema, "Attribute value > 1000 is rejected")


# ---------------------------------------------------------------------------
# Runner
# ---------------------------------------------------------------------------
def run_all_tests() -> None:
    print("\n" + "=" * 60)
    print("  W.H.A.T. I.F. — Character Schema Tests")
    print("  Phase 0 | Schema Validation Suite")
    print("=" * 60)

    print(f"\n[Setup] Loading schema from: {SCHEMA_PATH}")
    schema = load_json(SCHEMA_PATH)
    print(f"  ✅ Schema loaded ({len(json.dumps(schema))} bytes)\n")

    tests = [
        ("Schema Meta-Validation",         lambda: test_schema_is_valid_json_schema(schema)),
        ("Valid Sample Character",          lambda: test_valid_sample_character(schema)),
        ("Missing Required Field",          lambda: test_missing_character_id(schema)),
        ("Invalid Faction Enum",            lambda: test_invalid_faction_value(schema)),
        ("Personality Out of Range",        lambda: test_personality_out_of_range(schema)),
        ("Invalid character_id Format",     lambda: test_invalid_character_id_format(schema)),
        ("Relationship Affinity Range",     lambda: test_relationship_affinity_out_of_range(schema)),
        ("Minimal Valid Character",         lambda: test_minimal_valid_character(schema)),
        ("Attribute Value Max Range",       lambda: test_attribute_value_out_of_range(schema)),
    ]

    passed = 0
    failed = 0

    for name, fn in tests:
        print(f"[Test] {name}")
        try:
            fn()
            passed += 1
        except SystemExit:
            failed += 1
        except Exception as e:
            print(f"  ❌ FAIL  Unexpected exception: {e}")
            failed += 1
        print()

    print("=" * 60)
    print(f"  Results: {passed} passed, {failed} failed")
    print("=" * 60 + "\n")

    if failed > 0:
        sys.exit(1)


if __name__ == "__main__":
    run_all_tests()
