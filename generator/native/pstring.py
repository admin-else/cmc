from generator import template

class pstring:
  def __init__(self, data):
    self.impl, self.impl_h = template.gen_from("pstring", data)
