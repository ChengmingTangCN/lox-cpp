#!/usr/bin/python3

import sys
import json

depth = 0

def inc_file_print(arg, indent=True, delimiter=None):
  global depth
  if indent:
    inc_file.write("  " * depth)
  inc_file.write(arg)
  if delimiter:
    inc_file.write(delimiter)

def inc_file_println(arg = "", indent=True):
  inc_file_print(arg, indent, "\n")

def gen_inc_begin():
  inc_file_println("#pragma once")
  inc_file_println("#include \"scanner.h\"")
  inc_file_println()
  inc_file_println("#include <memory>")
  inc_file_println()
  inc_file_println("namespace Lox {")

def gen_inc_end():
  inc_file_println()
  inc_file_println("} // namespace Lox")

def gen_inc_defines(defines):
  inc_file_println()
  for line in defines:
    inc_file_println(line)

def gen_astnode_class():
  global depth

  inc_file_println("class AstNodeVisitor;")
  inc_file_println()

  inc_file_println("class AstNode {")
  inc_file_println("public:")
  depth += 1
  inc_file_println("virtual void accept(AstNodeVisitor &) = 0;")
  inc_file_println("virtual ~AstNode() noexcept = default;")
  depth -= 1
  inc_file_println("};")

def gen_astnode_visitor_class(node_classes):
  global depth

  inc_file_println()
  for node_class_name in node_classes:
    inc_file_println("class {};".format(node_class_name))

  inc_file_println()
  inc_file_println("class AstNodeVisitor {")
  inc_file_println("public:")
  depth += 1
  for node_class_name in node_classes:
      inc_file_println("virtual void visit({} &) = 0;".format(node_class_name))
  inc_file_println("virtual ~AstNodeVisitor() noexcept = default;")
  depth -= 1
  inc_file_println("};")

def gen_classes(classes):
  global depth

  gen_astnode_class()
  node_classes = []
  base_class_to_childs = {}

  for base_class_name in classes.keys():
    base_class_to_childs[base_class_name] = []
    inc_file_println()
    inc_file_println("class {} : public AstNode {{".format(base_class_name))
    inc_file_println("public:")
    depth += 1
    inc_file_println(" virtual ~{}() noexcept override = default;".format(base_class_name))
    depth -= 1
    inc_file_println("};")

    inc_file_println()
    for define in classes[base_class_name]["Defines"]:
      inc_file_println(define);

    for child_spec in classes[base_class_name]["Childs"]:
      items = child_spec.split()
      child_class_name = items[0]
      members = []
      for member in items[1:]:
        member = member.strip(",").strip()
        member_type, member_name = member.split(":")
        members.append({
          "name": member_name,
          "type": member_type,
        })
      node_classes.append(child_class_name)
      base_class_to_childs[base_class_name].append({"name": child_class_name, "members": members})

  gen_astnode_visitor_class(node_classes)

  for base_class_name in classes.keys():
      for child_class in base_class_to_childs[base_class_name]:
          child_class_name = child_class["name"]
          members = child_class["members"]
          inc_file_println(indent=False)
          inc_file_println("class {} final : public {} {{".format(child_class_name, base_class_name))
          inc_file_println("public:")
          depth += 1
          # ctor
          inc_file_print("{}(".format(child_class_name), indent=True)
          counter = 0
          for member in members:
            if counter > 0:
              inc_file_print(", ", indent=False)
            inc_file_print("{} {}".format(member["type"], member["name"]), indent=False)
            counter += 1
          inc_file_println(")", indent=False)
          inc_file_print(":   ", indent=True)
          counter = 0
          for member in members:
            if counter > 0:
              inc_file_print(", ", indent=False)
            if member["type"].endswith("Ptr"):
              inc_file_print("m_{0}(std::move({0}))".format(member["name"]), indent=False)
            else:
              inc_file_print("m_{0}({0})".format(member["name"]), indent=False)
            counter += 1
          inc_file_println(" {}")

          # visit
          inc_file_println(indent=False)
          inc_file_println("void accept(AstNodeVisitor &visitor) override {")
          depth += 1
          inc_file_println("visitor.visit(*this);")
          depth -= 1
          inc_file_println("}")

          # data members
          inc_file_println(indent=False)
          for member in members:
            inc_file_println("{} m_{};".format(member["type"], member["name"]))

          depth -= 1

          inc_file_println("};")

if __name__ == "__main__":
  if len(sys.argv) != 3:
    print("Usage: {} <json_path> <inc_path>".format(sys.argv[0]))
    sys.exit(1)

  json_path = sys.argv[1]
  inc_path = sys.argv[2]

  with open(json_path, "r") as json_file:
    json_data = json.load(json_file)
    defines = json_data["Defines"]
    classes = json_data["Classes"]

  with open(inc_path, "w+") as inc_file:
    gen_inc_begin()
    gen_inc_defines(defines)
    gen_classes(classes)
    gen_inc_end()
